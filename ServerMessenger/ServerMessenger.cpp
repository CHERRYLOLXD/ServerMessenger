#include "ServerMessenger.h"
#include "Server.h"

#include <iostream>

#include "dilithium/ref/api.h"
#include "dilithium/ref/params.h"
#include "dilithium/ref/randombytes.h"
#include "dilithium/ref/sign.h"

int main()
{
    spdlog::register_logger(std::make_shared<spdlog::logger>("server", spdlog::sinks_init_list{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>(), std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/server.log", 1024 * 1024 * 10, 5) }));
    
    std::jthread serverJThread(Server::Start);

    // Generate a public/private key pair
    uint8_t public_key[pqcrystals_dilithium5aes_ref_PUBLICKEYBYTES];
    uint8_t private_key[pqcrystals_dilithium5aes_ref_SECRETKEYBYTES];
    pqcrystals_dilithium5aes_ref_keypair(public_key, private_key);

    // Sign a message
    uint8_t message[] = "Hello, Dilithium!";
    uint8_t signature[pqcrystals_dilithium5aes_ref_BYTES];
    size_t signature_length;
    pqcrystals_dilithium5aes_ref_signature(signature, &signature_length, message, sizeof(message), private_key);

    // Verify the signature
    if (pqcrystals_dilithium5aes_ref_verify(signature, signature_length, message, sizeof(message), public_key))
    {
        std::cout << "Signature is valid" << std::endl;
    }
    else
    {
        std::cout << "Signature is invalid" << std::endl;
    }


    Server::Stop();
}
