idf_component_register(SRCS ./E2prom.c
                       INCLUDE_DIRS include                       # Edit following two lines to set component requirements (see docs)
                    #    REQUIRES )
                       PRIV_REQUIRES   crc8_16 nvs_flash)

