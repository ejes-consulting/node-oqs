/* src/binding.c:  */
#include <node_api.h>

extern napi_value get_signs(napi_env env, napi_callback_info info);
extern napi_value get_kems(napi_env env, napi_callback_info info);
extern napi_value create_kem(napi_env env, napi_callback_info info);
extern napi_value generate_keys(napi_env env, napi_callback_info info);
extern napi_value encapsulate(napi_env env, napi_callback_info info);
extern napi_value decapsulate(napi_env env, napi_callback_info info);
extern napi_value get_signs(napi_env env, napi_callback_info info);
extern napi_value create_sign(napi_env env, napi_callback_info info);
extern napi_value generate_signing_keys(napi_env env, napi_callback_info info);
extern napi_value sign(napi_env env, napi_callback_info info);
extern napi_value verify(napi_env env, napi_callback_info info);

// Initialize the module and export the function
napi_value Init(napi_env env, napi_value exports) {
	napi_status status;
    napi_value fn;

    // Define the exported function
    status=napi_create_function(env, "get_kems", NAPI_AUTO_LENGTH, get_kems, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'get_kems'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "get_kems", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'get_kems'");
        return NULL;
    }

    status=napi_create_function(env, "create_kem", NAPI_AUTO_LENGTH, create_kem, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'create_kem'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "create_kem", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'create_kem'");
        return NULL;
    }

    status=napi_create_function(env, "generate_keys", NAPI_AUTO_LENGTH, generate_keys, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'generate_keys'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "generate_keys", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'generate_keys'");
        return NULL;
    }

    // Export the encapsulate function
    status=napi_create_function(env, "encapsulate", NAPI_AUTO_LENGTH, encapsulate, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'encapsulate'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "encapsulate", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'encapsulate'");
        return NULL;
    }

    // Export the decapsulate function
    status=napi_create_function(env, "decapsulate", NAPI_AUTO_LENGTH, decapsulate, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'decapsulate'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "decapsulate", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'decapsulate'");
        return NULL;
    }

	// 

    // Define the exported function
    status=napi_create_function(env, "get_signs", NAPI_AUTO_LENGTH, get_signs, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'get_signs'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "get_signs", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'get_signs'");
        return NULL;
    }

    // Export the create_sign function
    status=napi_create_function(env, "create_sign", NAPI_AUTO_LENGTH, create_sign, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'create_sign'");
        return NULL;
    }
	status=napi_set_named_property(env, exports, "create_sign", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'create_sign'");
        return NULL;
    }

	// Export the sign_message function
    status=napi_create_function(env, "generate_signing_keys", NAPI_AUTO_LENGTH, generate_signing_keys, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'generate_signing_keys'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "generate_signing_keys", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'generate_signing_keys'");
        return NULL;
    }

    // Export the sign_message function
    status=napi_create_function(env, "sign", NAPI_AUTO_LENGTH, sign, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'sign'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "sign", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'sign'");
        return NULL;
    }

    // Export the sign_message function
    status=napi_create_function(env, "verify", NAPI_AUTO_LENGTH, verify, NULL, &fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_create_function", "Cannot create function 'verify'");
        return NULL;
    }
    status=napi_set_named_property(env, exports, "verify", fn);
    if (status != napi_ok ) {
        napi_throw_error(env, "napi_set_named_property", "Cannot set named property 'verify'");
        return NULL;
    }

    return exports;
}

/* eof */