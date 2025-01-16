/* src/signature.c: */

#include <node_api.h>
#include <oqs/oqs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// liboqs.get_signs(): returns Array of strings
napi_value get_signs(napi_env env, napi_callback_info info) {
    size_t sig_count = OQS_SIG_alg_count(); // Get the number of signature algorithms

    napi_value js_array;
    napi_create_array(env, &js_array); // Create a JS array for the algorithm names

    for (size_t i = 0; i < sig_count; i++) {
        const char *sig_name = OQS_SIG_alg_identifier(i);

        napi_value js_sig_name;
        napi_create_string_utf8(env, sig_name, NAPI_AUTO_LENGTH, &js_sig_name);

        napi_set_element(env, js_array, i, js_sig_name); // Add to JS array
    }

    return js_array; // Return the array
}

// Finalizer for OQS_SIG
void sign_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    OQS_SIG *sign = (OQS_SIG *)finalize_data;
    if (sign != NULL) {
        OQS_SIG_free(sign); // Free the OQS_SIG object
    }
}

// liboqs.create_sign(type: string): External
napi_value create_sign(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];

    napi_status status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok || argc < 1) {
        napi_throw_error(env, NULL, "Expected one argument: type");
        return NULL;
    }

    size_t type_len;
    napi_get_value_string_utf8(env, argv[0], NULL, 0, &type_len);
    char *type = (char *)malloc(type_len + 1);
    if (type == NULL) {
        napi_throw_error(env, NULL, "Memory allocation failed");
        return NULL;
    }
    napi_get_value_string_utf8(env, argv[0], type, type_len + 1, &type_len);

    OQS_SIG *sign = OQS_SIG_new(type);
    free(type);

    if (sign == NULL) {
        napi_throw_error(env, NULL, "Failed to create signature object");
        return NULL;
    }

    napi_value external;
    napi_create_external(env, sign, sign_finalize, NULL, &external);

    return external; // Return the external object
}

// liboqs.generate_signing_keys(sign: OQS_SIG): { publicKey: Buffer, privateKey: Buffer }
napi_value generate_signing_keys(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value argv[1];
    napi_value js_result_public, js_result_private;

    napi_status status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok || argc < 1) {
        napi_throw_error(env, NULL, "Expected one argument: sign");
        return NULL;
    }

    OQS_SIG *sign;
    napi_get_value_external(env, argv[0], (void **)&sign);

    size_t public_key_len = sign->length_public_key;
    size_t private_key_len = sign->length_secret_key;

    uint8_t *public_key = malloc(public_key_len);
    uint8_t *private_key = malloc(private_key_len);

    if (OQS_SIG_keypair(sign, public_key, private_key) != OQS_SUCCESS) {
        free(public_key);
        free(private_key);
        napi_throw_error(env, NULL, "Key pair generation failed");
        return NULL;
    }

    napi_create_buffer_copy(env, public_key_len, public_key, NULL, &js_result_public);
    napi_create_buffer_copy(env, private_key_len, private_key, NULL, &js_result_private);

    free(public_key);
    free(private_key);

    napi_value result;
    napi_create_object(env, &result);
    napi_set_named_property(env, result, "publicKey", js_result_public);
    napi_set_named_property(env, result, "privateKey", js_result_private);

    return result;
}

// liboqs.sign(sign: OQS_SIG, privateKey: Buffer, message: buffer): Buffer
napi_value sign(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value argv[3];
    napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    OQS_SIG *sign;
    napi_get_value_external(env, argv[0], (void **)&sign);

    const uint8_t *private_key;
    size_t private_key_len;
    napi_get_buffer_info(env, argv[1], (void **)&private_key, &private_key_len);

    const uint8_t *message;
    size_t message_len;
    napi_get_buffer_info(env, argv[2], (void **)&message, &message_len);

    size_t signature_len = sign->length_signature;
    uint8_t *signature = malloc(signature_len+1);

    if (OQS_SIG_sign(sign, signature, &signature_len, message, message_len, private_key) != OQS_SUCCESS) {
        free(signature);
        napi_throw_error(env, NULL, "Signature generation failed");
        return NULL;
    }

    napi_value js_result;
    napi_create_buffer_copy(env, signature_len, signature, NULL, &js_result);

    free(signature);

    return js_result;
}

// liboqs.verify(sign: OQS_SIG, publicKey: Buffer, signature: Buffer, message: buffer): boolean
napi_value verify(napi_env env, napi_callback_info info) {
    size_t argc = 4;
    napi_value argv[4];
    napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    OQS_SIG *sign;
    napi_get_value_external(env, argv[0], (void **)&sign);

    const uint8_t *public_key;
    size_t public_key_len;
    napi_get_buffer_info(env, argv[1], (void **)&public_key, &public_key_len);

    const uint8_t *signature;
    size_t signature_len;
    napi_get_buffer_info(env, argv[2], (void **)&signature, &signature_len);

    const uint8_t *message;
    size_t message_len;
    napi_get_buffer_info(env, argv[3], (void **)&message, &message_len);

	int result = OQS_SIG_verify(sign, message, message_len, signature, signature_len, public_key);

    napi_value js_result;
    napi_get_boolean(env, (result == OQS_SUCCESS), &js_result);

    return js_result;
}

/* eof */
