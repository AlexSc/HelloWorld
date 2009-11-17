from SCons.Script import *
from SCons.Tool import *
import Universal
from os import path

class Config:
   allConfigs = {}
   default = 'none'
   def __init__(self, name, default=False, suffix='', **kw):
      self.name = name
      self.opts = kw
      self.suffix = suffix
      Config.allConfigs[self.name] = self
      if default:
         Config.default = self.name

   def modenv(self, env):
      newEnv = env.Clone()
      newEnv.Prepend(**self.opts)
      return newEnv

def Build(environ, command, target, source, copy=True, *args, **kw):
   if environ['PLATFORM'] == 'win32':
      ld = SCons.Tool.createLoadableModuleBuilder(environ)
      ld.add_src_builder('RES')

      sl = SCons.Tool.createSharedLibBuilder(environ)
      sl.add_src_builder('RES')

      sl = SCons.Tool.createStaticLibBuilder(environ)
      sl.add_src_builder('RES')

      p = SCons.Tool.createProgBuilder(environ)
      p.add_src_builder('RES')

   configs = environ.subst('${cfg}').split()
   fullPath = {}
   cleanName = {}

   # Build each config
   for config in configs:
      config = Config.allConfigs[config]

      # Insert the configuration options
      newEnv = config.modenv(environ)

      cleanDict = {}
      # Do config substitution on incoming construction variables
      for key in kw.keys():
         if key.endswith('_CFG'):
            cleanDict[key[:-4]] = kw[key][config.name]
         else:
            cleanDict[key] = kw[key]

      outdir = 'build/' + config.name + '/'
      newFiles = []
      if SCons.Util.is_String(source):
         source = [source]

      # Tell SCons to build the files in a build/<cfg> directory
      dirs = []
      newSource = []
      for file in source:
         if SCons.Util.is_Dict(file):
            newSource.append(file[config.name])
         else:
            newSource.append(file)
         dir, file = path.split(str(file))
         newFiles.append(outdir + file)
         if not dir:
            dir = '.'
         if not dir in dirs:
            dirs.append(dir)

      source = newSource

      for dir in dirs:
         newEnv.VariantDir(outdir, dir, duplicate=0)

      newTarget = target + config.suffix
      p = newEnv.DoUniversal(command, outdir + newTarget, newFiles, *args, 
                             **cleanDict)

      # Copy the final product into our current working directory and save
      # that path for dependency tracking.
      if copy:
         fullPath[config.name] = newEnv.Command(path.split(str(p[0]))[1], p,
                                                Copy('$TARGET', '$SOURCE'))
      else:
         fullPath[config.name] = p

      # Save the unadorned name of the product to pass into other build comamnds
      cleanName[config.name] = newTarget

   return fullPath, cleanName

def GetVars(vars):
   vars.Add(ListVariable('cfg', 'Build configurations', Config.default,
                         Config.allConfigs.keys()))

AddMethod(Environment, Build)
