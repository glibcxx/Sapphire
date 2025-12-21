add_library(sapphire_bootloader SHARED
    Bootloader.cpp
    BootloaderMain.cpp
    RuntimeLinker.cpp
    SymbolResolver.cpp
    SigDatabase.cpp
    IatPatcher.cpp
)

sapphire_enable_release_pdb(sapphire_bootloader)

target_link_libraries(sapphire_bootloader PRIVATE SapphireCommon minhook)
target_compile_definitions(sapphire_bootloader PRIVATE "SRBL_DLLEXPORT")

install(TARGETS sapphire_bootloader
    EXPORT SapphireTargets
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
