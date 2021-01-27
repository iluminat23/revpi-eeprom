add_executable(revpi-eeprom revpi-eeprom.c)
install(TARGETS revpi-eeprom
    RUNTIME DESTINATION sbin
)