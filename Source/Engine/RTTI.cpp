#include "RTTI.h"

RTTIRepo* RTTIRepo::s_instance = nullptr;

RTTIRepo *RTTIRepo::instance()
{
    if (!s_instance)
        s_instance = new RTTIRepo();
    return s_instance;
}
