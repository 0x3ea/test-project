sysrepo_cpp_dep = dependency('sysrepo-cpp')
libyang_cpp_dep = dependency('libyang-cpp')

mobile_client_dep = [sysrepo_cpp_dep,
                     libyang_cpp_dep,
]

mobile_client_inc = include_directories('include')

mobile_client_src = ['source/netconf_agent.cpp',
                     'source/mobile_client.cpp',
]

mobile_client_lib = static_library('mobile_client_lib',
                                   sources : mobile_client_src,
                                   include_directories : mobile_client_inc,
                                   dependencies : mobile_client_dep,
)

mobile_client_lib_dep = declare_dependency(link_with : mobile_client_lib,
                                           include_directories : mobile_client_inc,
)
