# Begin code that does stuff
import Config
import DefaultConfig
import Universal

vars = Variables()
Config.GetVars(vars)
Universal.GetVars(vars)
env = Environment(variables=vars)

Export('env')

libs = env.SConscript('lib/SConscript')
progs = env.SConscript('cli/SConscript', 'libs')
plugin = env.SConscript('plugin/Hello/SConscript', 'libs')
