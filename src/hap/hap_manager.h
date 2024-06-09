#ifndef _HAP_MANAGER_H_
#define _HAP_MANAGER_H_

#include <FS.h>
#include <SPIFFS.h>

#include "../structs.h"

extern "C"
{
#include "homeintegration.h"
}

class hap_manager
{
public:
    hap_manager();

    void initialize(int button_num, bool reset = false);
    void button_callback(int button_id, ui_event_t event);

    bool get_homekit_paired_status();

private:
    void format_spiffs_();

    void init_hap_storage_(bool reset = false);

    static void storage_changed_(char *szstorage, int size);

    void notify_hap_(int button_id, uint8_t val);

    int buttons_count_ = 0;

    homekit_service_t **homekit_service_;

    const String pair_file_name_ = "/pair.dat";
};

#endif
