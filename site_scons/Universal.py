from SCons.Script import *
from os import path

def DoUniversal(env, command, target, source, *args, **kw):
   envs = []
   builder = env['BUILDERS'][command]
   exe = (command == 'LoadableModule' or command == 'SharedLibrary' or command == 'StaticLibrary' or command == 'Program' or command == 'Library')
   if env['PLATFORM'] == 'darwin' and exe:
      envs = [env.Clone(), env.Clone(), env.Clone()]

      envs[0].Append(CCFLAGS="-arch ppc", LINKFLAGS="-arch ppc",
                     OBJPREFIX='ppc')
      envs[1].Append(CCFLAGS="-arch i386", LINKFLAGS="-arch i386",
                     OBJPREFIX='i386')
      envs[2].Append(CCFLAGS="-arch x86_64", LINKFLAGS="-arch x86_64",
                     OBJPREFIX='x86_64')

      outs = []

      for environ in envs:
         outs += builder(environ, target=None, 
                         source=source, *args, **kw)
      p, f = path.split(target)
      target = path.join(p, builder.get_prefix(env) + f + builder.get_suffix(env))
      ret = env.Command(target, outs, "lipo -create $SOURCES -output $TARGET" )
   else:
      ret = builder(env, target, source, *args, **kw)

   return ret;

AddMethod(Environment, DoUniversal)
