/* liboqs-node.c:  liboqs node binding */
#include <node_api.h>

extern napi_value Init(napi_env env, napi_value exports);

NAPI_MODULE(liboqs_node, Init)

/* eof */
