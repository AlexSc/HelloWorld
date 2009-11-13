# Begin code that does stuff
import Config
import DefaultConfig

vars = Variables()
Config.GetVars(vars)
env = Environment(variables=vars)

Export('env')

libs = env.SConscript('lib/SConscript')

progs = env.SConscript('cli/SConscript', 'libs')
for cfg in libs[0].keys():
   Depends(progs[0][cfg], libs[0][cfg])

plugin = env.SConscript('plugin/Hello/SConscript', 'libs')
for cfg in libs[0].keys():
   Depends(plugin[0][cfg], libs[0][cfg])
   
