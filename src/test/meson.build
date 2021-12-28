gtest_dep = [dependency('gtest', main : true),
             dependency('gmock', main : true),
]

test_dep = [gtest_dep,
            mobile_client_lib_dep,
]

test_inc = include_directories('mock')

test_src = 'source/MobileClientTest.cpp'

mobile_network_test = executable('mobile_network_test',
                                 sources : test_src,
                                 include_directories : test_inc,
                                 dependencies : test_dep,
)

test('mobile_network_test', mobile_network_test)
