from SCons.Script import *
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

def Build(environ, command, target, source, *args, **kw):
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
      for file in source:
         dir, file = path.split(str(file))
         newFiles.append(outdir + file)
         if not dir:
            dir = '.'
         if not dir in dirs:
            dirs.append(dir)

      for dir in dirs:
         newEnv.VariantDir(outdir, dir, duplicate=0)
      builder = newEnv['BUILDERS'][command]
      newTarget = builder.get_prefix(newEnv) + target + config.suffix + builder.get_suffix(newEnv)
      newEnv.DoUniversal(command, outdir + newTarget, newFiles, *args, 
                         **cleanDict)
      fullPath[config.name] = newEnv.Command(newTarget, outdir + newTarget,
                                             Copy('$TARGET', '$SOURCE'))
      cleanName[config.name] = target +config.suffix

   return fullPath, cleanName

def GetVars(vars):
   vars.Add(ListVariable('cfg', 'Build configurations', Config.default,
                         Config.allConfigs.keys()))

AddMethod(Environment, Build)
