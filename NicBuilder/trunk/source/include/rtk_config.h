#ifndef __rtk_config_h__
#define __rtk_config_h__

/*Compatible macros
 */
#ifdef _WIN32
#define HAVE_STRUPR
#define HAVE_MILISECOND_SLEEP
#define HAVE_SNPRINTF 1
#define HAVE_PROFILE_SUPPORT
#endif

/* backward compatible */
#define CONFIG_FILE				"config/NicBuilder.ini"

#define rtk_vbus_node_key_length 16
#define VBUS_MAX_PACKET_SIZE 8192
#define VBUS_VERSION 0x0100
#define vbus_default_transaction_life 300
#define USE_MULTICAST 
#define VBUS_PORT_BASE			28434
#define VBUS_MULTICAST_IP_PREFIX	"234.1.1"
#define vbus_delay_between_fragments 10

#endif
