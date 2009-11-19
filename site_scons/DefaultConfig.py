import Config
from SCons.Script import *

dbgFlags = {}
optdbgFlags = {}
releaseFlags = {}

env = Environment()
if env['CC'] == 'gcc':
	dbgFlags['CCFLAGS'] = ['-g', '-O0']
	optdbgFlags['CCFLAGS'] = ['-g', '-O2']
	releaseFlags['CCFLAGS'] = ['-O2']
elif env['CC'] == 'cl':
	dbgFlags['CCFLAGS'] = ['/Zi', '/Od']
	optdbgFlags['CCFLAGS'] = ['/Zi', '/O2']
	releaseFlags['CCFLAGS'] = ['/O2']

dbg = Config.Config('dbg', default=True, suffix='_DEBUG', **dbgFlags)
optdbg = Config.Config('optdbg', suffix='_OPTIMIZED', **optdbgFlags)
release = Config.Config('release', **releaseFlags)
