/* src/kem.c:  provides Key Encapsulation Mechanism from liboqs */
#include <node_api.h>
#include <oqs/oqs.h>
#include <stdlib.h>
#include <string.h>

/* call in javascript liboqs.get_kems(): returns Array of strings */
napi_value get_kems(napi_env env, napi_callback_info info) {
    size_t kem_count=OQS_KEM_alg_count();
    
    napi_value js_array;
    napi_create_array(env,&js_array);

    for(size_t i=0;i<kem_count;i++) {
        const char *kem_name=OQS_KEM_alg_identifier(i);

        napi_value js_kem_name;
        napi_create_string_utf8(env,kem_name,NAPI_AUTO_LENGTH,&js_kem_name);

        napi_set_element(env,js_array,i,js_kem_name);
    }

  return js_array;
}

/* kem_finalize:  destructor for create_kmem */
static void kem_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    OQS_KEM *kem=(OQS_KEM *)finalize_data;
    OQS_KEM_free(kem);
    return;
}

/* liboqs.create_kem(type): returns Buffer of KEM */
napi_value create_kem(napi_env env, napi_callback_info info){
    napi_value argv[1];
    size_t argc = 1;
    napi_status status;
    size_t type_len;
    char type[256];

    // Parse the JavaScript argument (type)
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok || argc < 1) {
        napi_throw_error(env, "create_kem", "Expected one argument: type typeof 'string'");
        return NULL;
    }

    // Get the string length
    status = napi_get_value_string_utf8(env, argv[0], type, sizeof(type) - 1, &type_len);
    if (status != napi_ok) {
        napi_throw_error(env, "INVALID_ARGUMENT", "Failed to get type string");
        return NULL;
    }
    type[type_len] = '\0'; // Null-terminate the string

    // Create a new KEM instance
    OQS_KEM *kem = OQS_KEM_new(type);
    if (kem == NULL) {
        napi_throw_error(env, NULL, "Failed to create KEM instance");
        return NULL;
    }

    // Wrap the KEM instance in a JavaScript object
    napi_value kem_object;
    status = napi_create_external(env, kem, kem_finalize, NULL, &kem_object);
    if (status != napi_ok) {
        OQS_KEM_free(kem);
        napi_throw_error(env, NULL, "Failed to create KEM object");
        return NULL;
    }

    return kem_object;
}

// liboqs.generate_keys(kem_object): returns { publicKey: Buffer, privateKey: Buffer }
napi_value generate_keys(napi_env env, napi_callback_info info) {
    napi_value argv[1];
    size_t argc = 1;
    napi_status status;

    // Parse the JavaScript argument (kem_object)
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok || argc < 1) {
        napi_throw_error(env, NULL, "Expected one argument: KEM object");
        return NULL;
    }

    // Get the KEM instance from the object
    OQS_KEM *kem;
    status = napi_get_value_external(env, argv[0], (void **)&kem);
    if (status != napi_ok || kem == NULL) {
        napi_throw_error(env, NULL, "Invalid KEM object");
        return NULL;
    }

    // Allocate memory for keys
    uint8_t *public_key = malloc(kem->length_public_key);
    uint8_t *secret_key = malloc(kem->length_secret_key);
    if (public_key == NULL || secret_key == NULL) {
        free(public_key);
        free(secret_key);
        napi_throw_error(env, NULL, "Memory allocation failed for keys");
        return NULL;
    }

    // Generate the keypair
    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        free(public_key);
        free(secret_key);
        napi_throw_error(env, NULL, "Keypair generation failed");
        return NULL;
    }

    // Create result object
    napi_value result;
    napi_value public_key_buffer, private_key_buffer;

    napi_create_buffer_copy(env, kem->length_public_key, public_key, NULL, &public_key_buffer);
    napi_create_buffer_copy(env, kem->length_secret_key, secret_key, NULL, &private_key_buffer);

    napi_create_object(env, &result);
    napi_set_named_property(env, result, "publicKey", public_key_buffer);
    napi_set_named_property(env, result, "privateKey", private_key_buffer);

    // Cleanup
    free(public_key);
    free(secret_key);

    return result;
}

// liboqs.encapsulate(kem: Buffer, publicKey: Buffer): { ciphertext: Buffer, sharedSecret: Buffer }
napi_value encapsulate(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    if (argc != 2) {
        napi_throw_error(env, NULL, "Expected 2 arguments: kem , publicKey");
        return NULL;
    }

    // Get the KEM instance from the object
    OQS_KEM *kem;
    status = napi_get_value_external(env, args[0], (void **)&kem);
    if (status != napi_ok || kem == NULL) {
        napi_throw_error(env, NULL, "Invalid KEM object");
        return NULL;
    }

    // Get publicKey buffer
    uint8_t *public_key;
    size_t public_key_len;
    napi_get_buffer_info(env, args[1], (void **) &public_key, &public_key_len);

    if (public_key_len != kem->length_public_key) {
        napi_throw_error(env, NULL, "Invalid public key length");
        return NULL;
    }

    // Allocate memory for ciphertext and shared secret
    uint8_t *ciphertext = (uint8_t *) malloc(kem->length_ciphertext);
    uint8_t *shared_secret = (uint8_t *) malloc(kem->length_shared_secret);

    // Encapsulate
    if (OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key) != OQS_SUCCESS) {
        napi_throw_error(env, NULL, "Encapsulation failed");
        free(ciphertext);
        free(shared_secret);
        return NULL;
    }

    // Create return object
    napi_value result;
    napi_create_object(env, &result);

    // Add ciphertext buffer
    napi_value js_ciphertext;
    napi_create_buffer_copy(env, kem->length_ciphertext, ciphertext, NULL, &js_ciphertext);
    napi_set_named_property(env, result, "cipherText", js_ciphertext);

    // Add shared secret buffer
    napi_value js_shared_secret;
    napi_create_buffer_copy(env, kem->length_shared_secret, shared_secret, NULL, &js_shared_secret);
    napi_set_named_property(env, result, "sharedSecret", js_shared_secret);

    // Free resources
    free(ciphertext);
    free(shared_secret);

    return result;
}

// liboqs.decapsulate(kem:Buffer, ciphertext:Buffer, secretKey:Buffer): Buffer
napi_value decapsulate(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 3;
    napi_value args[3];
    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    if (argc != 3) {
        napi_throw_error(env, NULL, "Expected 3 arguments: kem, ciphertext, secretKey");
        return NULL;
    }

    // Get the KEM instance from the object
    OQS_KEM *kem;
    status = napi_get_value_external(env, args[0], (void **)&kem);
    if (status != napi_ok || kem == NULL) {
        napi_throw_error(env, NULL, "Invalid KEM object");
        return NULL;
    }

    // Get ciphertext buffer
    uint8_t *ciphertext;
    size_t ciphertext_len;
    napi_get_buffer_info(env, args[1], (void **) &ciphertext, &ciphertext_len);

    if (ciphertext_len != kem->length_ciphertext) {
        napi_throw_error(env, NULL, "Invalid ciphertext length");
        return NULL;
    }

    // Get secretKey buffer
    uint8_t *secret_key;
    size_t secret_key_len;
    napi_get_buffer_info(env, args[2], (void **) &secret_key, &secret_key_len);

    if (secret_key_len != kem->length_secret_key) {
        napi_throw_error(env, NULL, "Invalid secret key length");
        return NULL;
    }

    // Allocate memory for shared secret
    uint8_t *shared_secret = (uint8_t *) malloc(kem->length_shared_secret);

    // Decapsulate
    if (OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key) != OQS_SUCCESS) {
        napi_throw_error(env, NULL, "Decapsulation failed");
        free(shared_secret);
        return NULL;
    }

    // Create JavaScript Buffer for the shared secret
    napi_value js_shared_secret;
    napi_create_buffer_copy(env, kem->length_shared_secret, shared_secret, NULL, &js_shared_secret);

    // Free resources
    free(shared_secret);

    return js_shared_secret;
}

/* eof */