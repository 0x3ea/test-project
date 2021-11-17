netconf_agent_lib_inc = include_directories('include')

subdir('source')

netconf_agent_lib = static_library('netconf_agent_lib',
                                   sources : netconf_agent_lib_src,
                                   include_directories : netconf_agent_lib_inc,
)

netconf_agent_lib_dep = declare_dependency(link_with : netconf_agent_lib,
                                           include_directories : netconf_agent_lib_inc,
)
