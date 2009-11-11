/*
 *  plugin.cpp
 *  Hello
 *
 *  Created by Alex Scarborough on 11/9/09.
 *  Copyright 2009 GarageGames.com, Inc. All rights reserved.
 *
 */

#include "plugin.h"
#include "hello.h"
#include <string>
#include <stdio.h>

static NPNetscapeFuncs NPNFuncs;

void pluginInvalidate ();
bool pluginHasProperty (NPClass *theClass, NPIdentifier name);
bool pluginHasMethod (NPObject *npobj, NPIdentifier name);
bool pluginGetProperty (NPObject *obj, NPIdentifier name, NPVariant *variant);
bool pluginSetProperty (NPObject *obj, NPIdentifier name, const NPVariant *variant);
bool pluginInvoke (NPObject *obj, NPIdentifier name, NPVariant *args, uint32_t argCount, NPVariant *result);
bool pluginInvokeDefault (NPObject *obj, NPVariant *args, uint32_t argCount, NPVariant *result);

static NPClass _pluginFunctionPtrs = { 
   NP_CLASS_STRUCT_VERSION,
   (NPAllocateFunctionPtr)       NULL, 
   (NPDeallocateFunctionPtr)     NULL, 
   (NPInvalidateFunctionPtr)     pluginInvalidate,
   (NPHasMethodFunctionPtr)      pluginHasMethod,
   (NPInvokeFunctionPtr)         pluginInvoke,
   (NPInvokeDefaultFunctionPtr)  pluginInvokeDefault,
   (NPHasPropertyFunctionPtr)    pluginHasProperty,
   (NPGetPropertyFunctionPtr)    pluginGetProperty,
   (NPSetPropertyFunctionPtr)    pluginSetProperty,
};

#define ID_HELLO 0
#define NUM_METHOD_IDENTIFIERS 1

static NPIdentifier pluginMethodIdentifiers[NUM_METHOD_IDENTIFIERS];
static const NPUTF8 *pluginMethodIdentifierNames[NUM_METHOD_IDENTIFIERS] = {
   "i_say"
};

// Sets up the property and method identifier arrays used by the browser
// via the hasProperty and hasMethod fuction pointers
static void initializeIdentifiers()
{
   // fill the method identifier array
   NPNFuncs.getstringidentifiers(pluginMethodIdentifierNames,
      NUM_METHOD_IDENTIFIERS,
      pluginMethodIdentifiers);
};

bool pluginHasProperty (NPClass *theClass, NPIdentifier name)
{   
   return false;
}

bool pluginHasMethod(NPObject *npobj, NPIdentifier name)
{
	if (name == pluginMethodIdentifiers[ID_HELLO])
   	return true;
   return false;
}

bool pluginGetProperty (NPObject *obj, NPIdentifier name, NPVariant *variant)
{
   VOID_TO_NPVARIANT(*variant);

   return false;
}

bool pluginSetProperty (NPObject *obj, NPIdentifier name, const NPVariant *variant)
{
   return false;
}

void FillString(const std::string& src, NPVariant* variant)
{
   variant->type = NPVariantType_String;
   variant->value.stringValue.UTF8Length = static_cast<uint32_t>(src.length());
   variant->value.stringValue.UTF8Characters = reinterpret_cast<NPUTF8 *>(NPNFuncs.memalloc(src.size()));
   memcpy((void*)variant->value.stringValue.UTF8Characters, src.c_str(), src.size());   
}

// handle our plugin methods using standard np plugin conventions.
bool pluginInvoke (NPObject *obj, NPIdentifier name, NPVariant *args, unsigned argCount, NPVariant *result)
{
	fprintf(stderr, "[HelloWorld] - pluginInvoke\n");
   VOID_TO_NPVARIANT(*result);
   
   if (name == pluginMethodIdentifiers[ID_HELLO])
   {
   	hello Hello;
   	std::string value = Hello.i_say();
      FillString(value, result);
      return true;
   }

   return false;
}

bool pluginInvokeDefault (NPObject *obj, NPVariant *args, unsigned argCount, NPVariant *result)
{
   VOID_TO_NPVARIANT(*result);
   return false;
}

void pluginInvalidate ()
{
   // Make sure we've released any remaining references to JavaScript
   // objects.
}

NPError NP_Initialize(NPNetscapeFuncs *pFuncs)
{
	fprintf(stderr, "[HelloWorld] - NP_Initialize\n");
   if (pFuncs == NULL)
   {
   	fprintf(stderr, "[HelloWorld] - NPERR_INVALID_FUNCTABLE_ERROR\n");
      return NPERR_INVALID_FUNCTABLE_ERROR;
   }

   if ((pFuncs->version >> 8) > NP_VERSION_MAJOR)
   {
   	fprintf(stderr, "[HelloWorld] - NPERR_INCOMPATIBLE_VERSION_ERROR\n");
      return NPERR_INCOMPATIBLE_VERSION_ERROR;
   }

   // Safari sets the pfuncs size to 0
//   if (pFuncs->size == 0)
//      pFuncs->size = sizeof(NPNetscapeFuncs);
//   if (pFuncs->size < sizeof (NPNetscapeFuncs))
//   {
//   	fprintf(stderr, "[HelloWorld] - Too Small, %i vs %lu\n", pFuncs->size, sizeof(NPNetscapeFuncs));
//      return NPERR_INVALID_FUNCTABLE_ERROR;
//   }

   NPNFuncs = *pFuncs;
   
   fprintf(stderr, "[HelloWorld] - Sweet Success\n");

   return NPERR_NO_ERROR;
}

NPError NP_GetEntryPoints(NPPluginFuncs* pFuncs)
{
   if (pFuncs == NULL) {
      return NPERR_INVALID_FUNCTABLE_ERROR;
   }

   // Safari sets the size field of pFuncs to 0
   if (pFuncs->size == 0)
      pFuncs->size = sizeof(NPPluginFuncs);
   if (pFuncs->size < sizeof(NPPluginFuncs)) {
      return NPERR_INVALID_FUNCTABLE_ERROR;
   }
   
   fprintf(stderr, "[HelloWorld] - NP_GetEntryPoints\n");

   pFuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
   pFuncs->newp          = NPP_New;
   pFuncs->destroy       = NPP_Destroy;
   pFuncs->setwindow     = NPP_SetWindow;
   pFuncs->newstream     = NPP_NewStream;
   pFuncs->destroystream = NPP_DestroyStream;
   pFuncs->asfile        = NPP_StreamAsFile;
   pFuncs->writeready    = NPP_WriteReady;
   pFuncs->write         = NPP_Write;
   pFuncs->print         = NPP_Print;
   pFuncs->event         = NPP_HandleEvent;
   pFuncs->urlnotify     = NPP_URLNotify;
   pFuncs->getvalue      = NPP_GetValue;
   pFuncs->setvalue      = NPP_SetValue;
   pFuncs->javaClass     = NULL;

   return NPERR_NO_ERROR;
}

// Called to create a new instance of the plugin
NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, 
	int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
  return NPERR_NO_ERROR;
}

// Called to destroy an instance of the plugin
NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
  return NPERR_NO_ERROR;
}

// Called to update a plugin instances's NPWindow
NPError NPP_SetWindow(NPP instance, NPWindow* window)
{
  return NPERR_NO_ERROR;
}


NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, 
	NPBool seekable, uint16_t* stype)
{
  *stype = NP_ASFILEONLY;
  return NPERR_NO_ERROR;
}

NPError NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
  return NPERR_NO_ERROR;
}

int32_t NPP_WriteReady(NPP instance, NPStream* stream)
{
  return 0;
}

int32_t NPP_Write(NPP instance, NPStream* stream, int32_t offset, 
	int32_t len, void* buffer)
{
  return 0;
}

void NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
}

void NPP_Print(NPP instance, NPPrint* platformPrint)
{
  
}

int16_t NPP_HandleEvent(NPP instance, void* event)
{
  return 0;
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason, 
	void* notifyData)
{

}

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
	fprintf(stderr, "[HelloWorld] - GetValue\n");
	if (variable == NPPVpluginScriptableNPObject)
   {
   	fprintf(stderr, "[HelloWorld] - Scriptable\n");
   	static bool identifiersInitialized = false;
      if (!identifiersInitialized)
      {
         identifiersInitialized = true;
         initializeIdentifiers();
      }
      
   	if (instance->pdata == NULL)
      {
      	instance->pdata = NPNFuncs.createobject(instance, &_pluginFunctionPtrs);
      }
      
      NPObject* obj = reinterpret_cast<NPObject*> (instance->pdata);
      NPNFuncs.retainobject(obj);
      void **v = (void**)value;
      *v = obj;
      return NPERR_NO_ERROR;
   }

  return NPERR_GENERIC_ERROR;
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
  return NPERR_GENERIC_ERROR;
}

NPError NP_Shutdown()
{
	return NPERR_NO_ERROR;
}
