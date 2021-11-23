sysrepo_cpp_dep = dependency('sysrepo-cpp')
libyang_cpp_dep = dependency('libyang-cpp')

netconf_agent_dep = [sysrepo_cpp_dep,
                     libyang_cpp_dep,
]

netconf_agent_inc = include_directories('include')

subdir('source')

netconf_agent_lib = static_library('netconf_agent_lib',
                                   sources : netconf_agent_lib_src,
                                   include_directories : netconf_agent_inc,
                                   dependencies : netconf_agent_dep,
)

netconf_agent_lib_dep = declare_dependency(link_with : netconf_agent_lib,
                                           include_directories : netconf_agent_inc,
)
