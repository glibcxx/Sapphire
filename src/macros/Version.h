#pragma once

#define v1_21_2 1'21'002
#define v1_21_50 1'21'050
#define v1_21_60 1'21'060

#ifndef MC_VERSION
#    define MC_VERSION v1_21_50
#endif

#if !defined(SAPPHIRE_VERSION_MAJOR) || !defined(SAPPHIRE_VERSION_MINOR) || !defined(SAPPHIRE_VERSION_PATCH)
#    error "Sapphire version not set, please check CMakeLists.txt"
#endif

#ifndef SAPPHIRE_VERSION
#    define SAPPHIRE_VERSION SAPPHIRE_VERSION_MAJOR.SAPPHIRE_VERSION_MINOR.SAPPHIRE_VERSION_PATCH
#endif

#ifndef SAPPHIRE_VERSION_STRING
#    define SAPPHIRE_VERSION_STRING #SAPPHIRE_VERSION
#endif