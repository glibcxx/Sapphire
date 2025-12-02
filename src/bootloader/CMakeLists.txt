add_library(sapphire_bootloader SHARED
    BootloaderMain.cpp
    RuntimeLinker.cpp
    SymbolResolver.cpp
    SigDatabase.cpp
    IatPatcher.cpp
)

target_link_libraries(sapphire_bootloader PRIVATE SapphireCommon minhook)

install(TARGETS sapphire_bootloader
    RUNTIME DESTINATION Sapphire/bin/ COMPONENT sapphire_bootloader
)
install(FILES $<TARGET_PDB_FILE:sapphire_bootloader>
    DESTINATION Sapphire/bin/ COMPONENT sapphire_bootloader
    OPTIONAL
)

sapphire_add_install_target(
    install-sapphire_bootloader
    COMPONENT sapphire_bootloader
)
