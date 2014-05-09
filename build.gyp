{
  'targets': [
    {
      'target_name': 'asmjit',
      'type': 'static_library',
      'include_dirs': ['src'],
      'direct_dependent_settings': {
        'include_dirs': ['src'],
      },
      'dependencies': ['base', 'masm'],
      'sources': [
        'src/asmjit/base/compiler.cpp',
        'src/asmjit/base/compiler.h',
        'src/asmjit/base/constpool.cpp',
        'src/asmjit/base/constpool.h',
        'src/asmjit/base/context.cpp',
        'src/asmjit/base/context_p.h',
        'src/asmjit/base/cpuinfo.cpp',
        'src/asmjit/base/cpuinfo.h',
        'src/asmjit/base/cputicks.cpp',
        'src/asmjit/base/cputicks.h',
        'src/asmjit/base/func.cpp',
        'src/asmjit/base/func.h',
        'src/asmjit/base/intutil.h',
        'src/asmjit/base/lock.h',
        'src/asmjit/base/runtime.cpp',
        'src/asmjit/base/runtime.h',
        'src/asmjit/base/vectypes.h',
        'src/asmjit/base/vmem.cpp',
        'src/asmjit/base/vmem.h',
      ],
    },
    {
      'target_name': 'base',
      'type': 'static_library',
      'defines': ['ASMJIT_DEBUG'],
      'include_dirs': ['src'],
      'direct_dependent_settings': {
        'defines': ['ASMJIT_DEBUG'],
        'include_dirs': ['src'],
      },
      'sources': [
        'src/asmjit/base/codegen.cpp',
        'src/asmjit/base/codegen.h',
        'src/asmjit/base/defs.cpp',
        'src/asmjit/base/defs.h',
        'src/asmjit/base/error.cpp',
        'src/asmjit/base/error.h',
        'src/asmjit/base/globals.cpp',
        'src/asmjit/base/globals.h',
        'src/asmjit/base/logger.cpp',
        'src/asmjit/base/logger.h',
        'src/asmjit/base/podlist.h',
        'src/asmjit/base/podvector.cpp',
        'src/asmjit/base/podvector.h',
        'src/asmjit/base/string.cpp',
        'src/asmjit/base/string.h',
        'src/asmjit/base/zone.cpp',
        'src/asmjit/base/zone.h',
      ],
    },
    {
      'target_name': 'masm',
      'type': 'static_library',
      'include_dirs': ['src'],
      'direct_dependent_settings': {
        'include_dirs': ['src'],
      },
      'dependencies': ['base'],
      'sources': [
        'src/asmjit/base/assembler.cpp',
        'src/asmjit/base/assembler.h',
        'src/asmjit/x86/x86assembler.cpp',
        'src/asmjit/x86/x86assembler.h',
        'src/asmjit/x86/x86compiler.cpp',
        'src/asmjit/x86/x86compiler.h',
        'src/asmjit/x86/x86context.cpp',
        'src/asmjit/x86/x86context_p.h',
        'src/asmjit/x86/x86cpuinfo.cpp',
        'src/asmjit/x86/x86cpuinfo.h',
        'src/asmjit/x86/x86defs.cpp',
        'src/asmjit/x86/x86defs.h',
        'src/asmjit/x86/x86func.cpp',
        'src/asmjit/x86/x86func.h',
      ],
    },
  ]
}
