#ifndef ECO_ECO_H
#define ECO_ECO_H

#include <ecore/base/defs.h>
#include <ecore/base/any.h>
#include <ecore/io/logging/log.h>

/*
 *
 *    O b j e c t   a n d   T y p e   D e c l a r a t i o n s
 *
 */

struct Eco_Object;
struct Eco_Type;


/*
 *
 *    E c o   M a i n   F u n c t i o n s
 *
 */

void Eco_Init();
void Eco_Terminate();
void Eco_LoadImageFromFile(const char*);
void Eco_Run();

#endif
