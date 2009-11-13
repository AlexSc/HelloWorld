from SCons.Script import *

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
   for config in configs:
      config = Config.allConfigs[config]
      cleanDict = {}
      for key in kw.keys():
         if key.endswith('_CFG'):
           cleanDict[key[:-4]] = kw[key][config.name]
         else:
         	cleanDict[key] = kw[key]
      outdir = 'build/' + config.name + '/'
      newFiles = []
      if SCons.Util.is_String(source):
         source = [source]
      for file in source:
         newFiles.append(outdir + file)
      newEnv = config.modenv(environ)
      newTarget = newEnv['BUILDERS'][command].get_prefix(newEnv) + target + config.suffix + newEnv['BUILDERS'][command].get_suffix(newEnv)
      newEnv.VariantDir(outdir, '.', duplicate=0)
      newEnv.DoUniversal(command, outdir + newTarget, newFiles, *args, **cleanDict)
      fullPath[config.name] = newEnv.Command(newTarget, outdir + newTarget, Copy('$TARGET', '$SOURCE'))
      cleanName[config.name] = target +config.suffix
   
   return fullPath, cleanName
   
def GetVars(vars):
	vars.Add(ListVariable('cfg', 'Build configurations', Config.default, Config.allConfigs.keys()))
   
AddMethod(Environment, Build)
