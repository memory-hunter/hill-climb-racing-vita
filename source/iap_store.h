#ifndef SOLOADER_IAP_STORE_H
#define SOLOADER_IAP_STORE_H

#include <falso_jni/jni.h>

#ifdef __cplusplus
extern "C" {
#endif

void iap_purchase(const char *sku);

int iap_get_coins(void);
int iap_get_gems(void);
int iap_get_adfree(void);
int iap_get_bundle(void);
int iap_get_error(void);

void iap_reset(void);

#ifdef __cplusplus
}
#endif

#endif // SOLOADER_IAP_STORE_H
