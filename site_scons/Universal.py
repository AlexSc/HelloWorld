from SCons.Script import *

def DoUniversal(env, command, target, source, *args, **kw):
   envs = []
   if env['PLATFORM'] == 'darwin':
      envs = [env.Clone(), env.Clone(), env.Clone()]

      envs[0].Append(CCFLAGS="-arch ppc", LINKFLAGS="-arch ppc", OBJPREFIX='ppc')
      envs[1].Append(CCFLAGS="-arch i386", LINKFLAGS="-arch i386", OBJPREFIX='i386')
      envs[2].Append(CCFLAGS="-arch x86_64", LINKFLAGS="-arch x86_64", OBJPREFIX='x86_64')

      outs = []

      for environ in envs:
         outs += environ['BUILDERS'][command](environ, target=None, source=source, *args, **kw)
      env.Command(target, outs, "lipo -create $SOURCES -output $TARGET" )
   else:
      env['BUILDERS'][command](env, target, source, *args, **kw)

   return None;
   
AddMethod(Environment, DoUniversal)
