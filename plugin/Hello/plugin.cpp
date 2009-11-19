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
#include <string.h>
#include <stdio.h>

static NPNetscapeFuncs NPNFuncs;

// MARK: Initialization

#ifdef XP_UNIX
// Unix uses the in functions and out functions.  Due to a bug in the SDK
// this signature is also required on OS X, but do not use outFuncs as that
// will crash.
NPError NP_Initialize(NPNetscapeFuncs *pFuncs, NPPluginFuncs* outFuncs)
#else
// On Windows we only receive the in functions and need to use OSCALL
// (defined as WINAPI) to ensure we have the correct calling convention for
// an exported function.
NPError OSCALL NP_Initialize(NPNetscapeFuncs *pFuncs)
#endif
{
   // You never know.
   if (pFuncs == NULL)
   {
      return NPERR_INVALID_FUNCTABLE_ERROR;
   }

   // Major versions are assumed to completely break compatibility.  If this is
   // a different major version, don't even try.
   if ((pFuncs->version >> 8) != NP_VERSION_MAJOR)
   {
      return NPERR_INCOMPATIBLE_VERSION_ERROR;
   }
   
   // If the brower's supported minor version is less than the version we were
   // compiled against, assume we are incompatible.
   if (((int)(char)pFuncs->version) < NP_VERSION_MINOR)
   {
      return NPERR_INCOMPATIBLE_VERSION_ERROR;
   }

   NPNFuncs = *pFuncs;

#if defined(XP_UNIX) && !defined(XP_MACOSX)
   // On Unix type systems (except for OS X) NP_GetEntryPoints won't be called
   // so call it ourselves.
   NP_GetEntryPoints(outFuncs);
#endif

   // You can haz plugin.
   return NPERR_NO_ERROR;
}

// This method will be called on Unix and on OS X.
// On Unix, this function is the only way to get MIME type information from a
// plugin, and this function must be implemented.  On OS X the information
// returned from this function overrides the information from the Info.plist
// file in the plugin bundle.  This function is optional on OS X.
char* NP_GetMIMEDescription()
{
   return MIME_TYPE"::Hello World Plugin";
}

// This function is called by the browser on Windows and on OS X.
// It is responsible for filling in a function table which the browser will
// use to call into the plugin.  On Unix this table is passed in to
// NP_Initialize and must be filled in there.
//
// In this sample our NP_Initialize function calls NP_GetEntryPoints on Unix.
// It's the simplest solution if you're writing a xplatform plugin.
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* pFuncs)
{
   // You never know.
   if (pFuncs == NULL) 
   {
      return NPERR_INVALID_FUNCTABLE_ERROR;
   }
   
   // If our version was incompatible with the plugin version, we shouldn't get
   // here, so don't check version info again.

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

// This function is called on Unix to retrieve a human readable plugin name
// and description to display in an about page or help page or otherwise
// (you get the idea).
//
// The first parameter would be an NPP instance, the idea being that you could
// use NP_GetValue as your NP*P*_GetValue. I have chosen not to do this for
// clarity.  NP_GetValue is explicitly exported on Unix and the plugin loads
// this function directly.
NPError OSCALL NP_GetValue(void*, NPPVariable variable, void* value)
{
   // Return the human readable name of our plugin.
   if (variable == NPPVpluginNameString)
   {
      const char** val = (const char**)value;
      *val = "Hello World Plugin";
      return NPERR_NO_ERROR;
   }

   // Return a human readable description of what the plugin does.
   if (variable == NPPVpluginDescriptionString)
   {
      const char** val = (const char**)value;
      // In your own plugin, make this a bit more descriptive :p
      *val = "Hello World Plugin";
      return NPERR_NO_ERROR;
   }

   // We don't recognize any other parameters.  If you're not using NP_GetValue
   // as NP*P*_GetValue, then it shouldn't be called with any other parameters
   // anyways.
   return NPERR_INVALID_PARAM;
}

// MARK: -
// MARK: Instance Methods
// None of these are implemented in this example plugin, as it does not do
// any special per-instance functionality.  Your own plugin probably will.
// More information on how to implement these functions is outside the scope
// of this example.
// Called to create a new instance of the plugin
NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16 mode,
   int16 argc, char* argn[], char* argv[], NPSavedData* saved)
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
   NPBool seekable, uint16* stype)
{
   *stype = NP_ASFILEONLY;
   return NPERR_NO_ERROR;
}

NPError NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
   return NPERR_NO_ERROR;
}

int32 NPP_WriteReady(NPP instance, NPStream* stream)
{
   return 0;
}

int32 NPP_Write(NPP instance, NPStream* stream, int32 offset, 
   int32 len, void* buffer)
{
   return 0;
}

void NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
}

void NPP_Print(NPP instance, NPPrint* platformPrint)
{

}

int16 NPP_HandleEvent(NPP instance, void* event)
{
   return 0;
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason,
   void* notifyData)
{

}

// MARK: -
// MARK: Scripting

static void initializeIdentifiers();
void pluginInvalidate ();
bool pluginHasProperty (NPClass *theClass, NPIdentifier name);
bool pluginHasMethod (NPObject *npobj, NPIdentifier name);
bool pluginGetProperty (NPObject *obj, NPIdentifier name, NPVariant *variant);
bool pluginSetProperty (NPObject *obj, NPIdentifier name,
                        const NPVariant *variant);
bool pluginInvoke (NPObject *obj, NPIdentifier name, NPVariant *args,
                   uint32_t argCount, NPVariant *result);
bool pluginInvokeDefault (NPObject *obj, NPVariant *args, uint32_t argCount,
                          NPVariant *result);

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

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
   if (variable == NPPVpluginScriptableNPObject)
   {
      static bool identifiersInitialized = false;
      if (!identifiersInitialized)
      {
         identifiersInitialized = true;
         initializeIdentifiers();
      }

      if (instance->pdata == NULL)
      {
         instance->pdata = NPNFuncs.createobject(instance,
                                                 &_pluginFunctionPtrs);
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

bool pluginHasProperty(NPClass *theClass, NPIdentifier name)
{
   return false;
}

bool pluginHasMethod(NPObject *npobj, NPIdentifier name)
{
   if (name == pluginMethodIdentifiers[ID_HELLO])
      return true;
   return false;
}

bool pluginGetProperty(NPObject *obj, NPIdentifier name, NPVariant *variant)
{
   VOID_TO_NPVARIANT(*variant);

   return false;
}

bool pluginSetProperty(NPObject *obj, NPIdentifier name,
                        const NPVariant *variant)
{
   return false;
}

void FillString(const std::string& src, NPVariant* variant)
{
   variant->type = NPVariantType_String;
   variant->value.stringValue.utf8length = static_cast<uint32_t>(src.length());
   variant->value.stringValue.utf8characters = reinterpret_cast<NPUTF8 *>
                                               (NPNFuncs.memalloc(src.size()));
   memcpy((void*)variant->value.stringValue.utf8characters, src.c_str(),
          src.size());
}

// handle our plugin methods using standard np plugin conventions.
bool pluginInvoke(NPObject *obj, NPIdentifier name, NPVariant *args,
                   unsigned argCount, NPVariant *result)
{
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

bool pluginInvokeDefault(NPObject *obj, NPVariant *args, unsigned argCount,
                          NPVariant *result)
{
   VOID_TO_NPVARIANT(*result);
   return false;
}

void pluginInvalidate()
{
   // Make sure we've released any remaining references to JavaScript
   // objects.
}

// MARK: -
// MARK: Shutdown

// This function is called after every instance of your plugin is unloaded.
// This function is (thankfully) called at the same time on every platform
// with the same signature.
NPError OSCALL NP_Shutdown()
{
   return NPERR_NO_ERROR;
}
