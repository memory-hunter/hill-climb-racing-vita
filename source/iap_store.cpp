#include "iap_store.h"
#include "utils/logger.h"

#include <string.h>

typedef struct {
    const char *sku;
    int coins;
    int gems;
    int bundle;
} IapItem;

static const IapItem iap_catalog[] = {
    // iap1 coins
    {"com.fingersoft.hillclimb.adfree_150000coins", 150000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_300000coins", 300000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_750000coins", 750000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_2000000coins", 2000000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_4000000coins", 4000000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_8000000coins", 8000000, 0, 0},
    {"com.fingersoft.hillclimb.adfree_20000000coins", 20000000, 0, 0},
    // iap2 coins
    {"com.fingersoft.hillclimb.iap2.adfree_300000coins", 300000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_600000coins", 600000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_1500000coins", 1500000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_4000000coins", 4000000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_8000000coins", 8000000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_16000000coins", 16000000, 0, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_40000000coins", 40000000, 0, 0},
    // iap3 coins
    {"com.fingersoft.hillclimb.iap3.adfree_600000coins", 600000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_1200000coins", 1200000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_3000000coins", 3000000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_8000000coins", 8000000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_16000000coins", 16000000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_32000000coins", 32000000, 0, 0},
    {"com.fingersoft.hillclimb.iap3.adfree_80000000coins", 80000000, 0, 0},
    // iap4 coins
    {"com.fingersoft.hillclimb.iap4.adfree_1200000coins", 1200000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_2400000coins", 2400000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_6000000coins", 6000000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_16000000coins", 16000000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_32000000coins", 32000000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_64000000coins", 64000000, 0, 0},
    {"com.fingersoft.hillclimb.iap4.adfree_160000000coins", 160000000, 0, 0},
    // iap2 gems
    {"com.fingersoft.hillclimb.iap2.adfree_500gems", 0, 500, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_2000gems", 0, 2000, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_5000gems", 0, 5000, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_11000gems", 0, 11000, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_23000gems", 0, 23000, 0},
    {"com.fingersoft.hillclimb.iap2.adfree_40000gems", 0, 40000, 0},
    // iap1 gems
    {"com.fingersoft.hillclimb.iap1.adfree_1000gems", 0, 1000, 0},
    {"com.fingersoft.hillclimb.iap1.adfree_3000gems", 0, 3000, 0},
    {"com.fingersoft.hillclimb.iap1.adfree_6000gems", 0, 6000, 0},
    {"com.fingersoft.hillclimb.iap1.adfree_15000gems", 0, 15000, 0},
    {"com.fingersoft.hillclimb.iap1.adfree_26000gems", 0, 26000, 0},
    {"com.fingersoft.hillclimb.iap1.adfree_50000gems", 0, 50000, 0},
    // special offers
    {"com.fingersoft.hillclimb.specialoffer1", 100000000, 0, 0},
    {"com.fingersoft.hillclimb.specialoffer2", 20000000, 0, 0},
    {"com.fingersoft.hillclimb.specialoffer3", 5000000, 0, 0},
    {"com.fingersoft.hillclimb.specialoffer1gems", 0, 30000, 0},
    // bundles
    {"com.fingersoft.hillclimb.special_garage1", 0, 0, 1},
    {"com.fingersoft.hillclimb.special_garage2", 0, 0, 2},
    {"com.fingersoft.hillclimb.bundle10", 0, 0, 10},
    {"com.fingersoft.hillclimb.bundle11", 0, 0, 11},
    {"com.fingersoft.hillclimb.bundle12", 0, 0, 12},
    {"com.fingersoft.hillclimb.bundle13", 0, 0, 13},
    {"com.fingersoft.hillclimb.bundle14", 0, 0, 14},
    {"com.fingersoft.hillclimb.bundle15", 0, 0, 15},
    {"com.fingersoft.hillclimb.bundle16", 0, 0, 16},
    {"com.fingersoft.hillclimb.bundle17", 0, 0, 17},
    {"com.fingersoft.hillclimb.bundle18", 0, 0, 18},
    {"com.fingersoft.hillclimb.bundle19", 0, 0, 19},
    {"com.fingersoft.hillclimb.bundle20", 0, 0, 20},
    {"com.fingersoft.hillclimb.bundle21", 0, 0, 21},
};

static const int iap_catalog_size = sizeof(iap_catalog) / sizeof(iap_catalog[0]);

static int pending_coins = 0;
static int pending_gems = 0;
static int pending_adfree = 0;
static int pending_bundle = 0;
static int pending_error = 0;

void iap_purchase(const char *sku)
{
    for (int i = 0; i < iap_catalog_size; i++) {
        if (strcmp(sku, iap_catalog[i].sku) == 0) {
            pending_coins += iap_catalog[i].coins;
            pending_gems += iap_catalog[i].gems;
            pending_adfree = 1;
            if (iap_catalog[i].bundle > 0)
                pending_bundle = iap_catalog[i].bundle;
            l_info("IAP purchase: %s -> coins=%d gems=%d bundle=%d",
                   sku, iap_catalog[i].coins, iap_catalog[i].gems, iap_catalog[i].bundle);
            return;
        }
    }
    l_warn("IAP purchase: unknown SKU '%s'", sku);
    pending_error = 1;
}

int iap_get_coins(void)
{
    return pending_coins;
}

int iap_get_gems(void)
{
    return pending_gems;
}

int iap_get_adfree(void)
{
    return pending_adfree;
}

int iap_get_bundle(void)
{
    return pending_bundle;
}

int iap_get_error(void)
{
    return pending_error;
}

void iap_reset(void)
{
    pending_coins = 0;
    pending_gems = 0;
    pending_adfree = 0;
    pending_bundle = 0;
    pending_error = 0;
    l_info("IAP store reset");
}
