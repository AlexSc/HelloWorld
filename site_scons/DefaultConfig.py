import Config

dbg = Config.Config('dbg', default=True, CCFLAGS=['-g', '-O0'], suffix='_DEBUG')
optdbg = Config.Config('optdbg', CCFLAGS=['-g', '-02'], suffix='_OPTIMIZED')
release = Config.Config('release', CCFLAGS=['-02'])
