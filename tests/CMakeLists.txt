#TEST 1
add_executable(test_key "key_expansion_test.cpp")
target_link_libraries(test_key PRIVATE 
	OpenCL_crypto_lib
)  
add_test(NAME test_key COMMAND test_key)

#TEST 2
add_executable(test_enc "encrypt_test.cpp")
target_link_libraries(test_enc PRIVATE 
	OpenCL_crypto_lib
)  
add_test(NAME test_enc COMMAND test_enc)