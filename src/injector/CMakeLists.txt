add_executable(sapphire_launcher UWPinjector.cpp sapphire.rc)

target_link_libraries(sapphire_launcher PRIVATE SapphireCommon)

install(TARGETS sapphire_launcher
    DESTINATION Sapphire/
    COMPONENT sapphire_launcher
)
install(FILES $<TARGET_PDB_FILE:sapphire_launcher>
    DESTINATION Sapphire/ COMPONENT sapphire_launcher
    OPTIONAL
)

sapphire_add_install_target(install-sapphire_launcher COMPONENT sapphire_launcher)
