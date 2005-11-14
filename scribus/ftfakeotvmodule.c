#ifndef __FAKE_OTV_MODULE_H__
#define __FAKE_OTV_MODULE_H__

#include <ft2build.h>
#include FT_INTERNAL_OBJECTS_H
#include FT_MODULE_H

FT_BEGIN_HEADER

FT_CALLBACK_TABLE_DEF
const FT_Module_Class  otv_module_class =
{
    0,
    sizeof( FT_ModuleRec ),
    "otvalid",
    0x10000L,
    0x20000L,
    
    0,              /* module-specific interface */
    
    (FT_Module_Constructor)0,
    (FT_Module_Destructor) 0,
    (FT_Module_Requester) 0
};

FT_END_HEADER

#endif /* __FAKE_OTV_MODULE_H__ */
