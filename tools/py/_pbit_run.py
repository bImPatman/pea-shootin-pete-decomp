import os, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'py'))
import bcbuild

ROOT = bcbuild.ROOT
src = os.path.join(ROOT, 'src', '_pbit.c')
r = bcbuild.build([src], name='pbit', want_asm=True, keep=True)
print('ok', r['ok'], 'rc', r['rc'])
print(r['log'][-500:])
