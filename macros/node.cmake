set(NODEQUAN_SOURCES
        "external/quan-node/external/miniupnpc/connecthostport.c"
        "external/quan-node/external/miniupnpc/igd_desc_parse.c"
        "external/quan-node/external/miniupnpc/minisoap.c"
        "external/quan-node/external/miniupnpc/miniupnpc.c"
        "external/quan-node/external/miniupnpc/miniwget.c"
        "external/quan-node/external/miniupnpc/minixml.c"
        "external/quan-node/external/miniupnpc/portlistingparse.c"
        "external/quan-node/external/miniupnpc/receivedata.c"
        "external/quan-node/external/miniupnpc/upnpcommands.c"
        "external/quan-node/external/miniupnpc/upnpreplyparse.c"
        "external/quan-node/src/Common/Base58.cpp"
        "external/quan-node/src/Common/Base64.cpp"
        "external/quan-node/src/Common/CommandLine.cpp"
        "external/quan-node/src/Common/Util.cpp"
        "external/quan-node/src/Common/StringTools.cpp"
        "external/quan-node/src/Common/JsonValue.cpp"
        "external/quan-node/src/Common/ConsoleTools.cpp"
        "external/quan-node/src/Common/MemoryInputStream.cpp"
        "external/quan-node/src/Common/PathTools.cpp"
        "external/quan-node/src/Common/DnsTools.cpp"
        "external/quan-node/src/Common/StdInputStream.cpp"
        "external/quan-node/src/Common/StdOutputStream.cpp"
        "external/quan-node/src/Common/StreamTools.cpp"
        "external/quan-node/src/Common/StringOutputStream.cpp"
        "external/quan-node/src/Common/StringView.cpp"
        "external/quan-node/src/Common/VectorOutputStream.cpp"
        "external/quan-node/src/crypto/blake256.c"
        "external/quan-node/src/crypto/chacha8.c"
        "external/quan-node/src/crypto/crypto-ops-data.c"
        "external/quan-node/src/crypto/crypto-ops.c"
        "external/quan-node/src/crypto/crypto.cpp"
        "external/quan-node/src/crypto/groestl.c"
        "external/quan-node/src/crypto/hash.c"
        "external/quan-node/src/crypto/jh.c"
        "external/quan-node/src/crypto/keccak.c"
        "external/quan-node/src/crypto/random.c"
        "external/quan-node/src/crypto/skein.c"
        "external/quan-node/src/crypto/cn_slow_hash_soft.cpp"
        "external/quan-node/src/crypto/cn_slow_hash_hard_intel.cpp"
        "external/quan-node/src/crypto/cn_slow_hash_hard_arm.cpp"
        "external/quan-node/src/crypto/tree-hash.c"
        "external/quan-node/src/CryptoNoteCore/BlockchainIndices.cpp"
        "external/quan-node/src/CryptoNoteCore/BlockchainMessages.cpp"
        "external/quan-node/src/CryptoNoteCore/BlockIndex.cpp"
        "external/quan-node/src/CryptoNoteCore/CoreConfig.cpp"
        "external/quan-node/src/CryptoNoteCore/CryptoNoteBasic.cpp"
        "external/quan-node/src/CryptoNoteCore/CryptoNoteTools.cpp"
        "external/quan-node/src/CryptoNoteCore/Currency.cpp"
        "external/quan-node/src/CryptoNoteCore/MinerConfig.cpp"
        "external/quan-node/src/CryptoNoteCore/Transaction.cpp"
        "external/quan-node/src/CryptoNoteCore/Account.cpp"
        "external/quan-node/src/CryptoNoteCore/Blockchain.cpp"
        "external/quan-node/src/CryptoNoteCore/Checkpoints.cpp"
        "external/quan-node/src/CryptoNoteCore/CryptoNoteBasicImpl.cpp"
        "external/quan-node/src/CryptoNoteCore/Core.cpp"
        "external/quan-node/src/CryptoNoteCore/CryptoNoteFormatUtils.cpp"
        "external/quan-node/src/CryptoNoteCore/CryptoNoteSerialization.cpp"
        "external/quan-node/src/CryptoNoteCore/Difficulty.cpp"
        "external/quan-node/src/CryptoNoteCore/IBlock.cpp"
        "external/quan-node/src/CryptoNoteCore/Miner.cpp"
        "external/quan-node/src/CryptoNoteCore/TransactionExtra.cpp"
        "external/quan-node/src/CryptoNoteCore/TransactionPool.cpp"
        "external/quan-node/src/CryptoNoteCore/TransactionPrefixImpl.cpp"
        "external/quan-node/src/CryptoNoteCore/TransactionUtils.cpp"
        "external/quan-node/src/InProcessNode/InProcessNode.cpp"
        "external/quan-node/src/InProcessNode/InProcessNodeErrors.cpp"
        "external/quan-node/src/NodeRpcProxy/NodeErrors.cpp"
        "external/quan-node/src/NodeRpcProxy/NodeRpcProxy.cpp"
        "external/quan-node/src/P2p/NetNodeConfig.cpp"
        "external/quan-node/src/Serialization/BinaryInputStreamSerializer.cpp"
        "external/quan-node/src/Serialization/BinaryOutputStreamSerializer.cpp"
        "external/quan-node/src/Serialization/JsonInputValueSerializer.cpp"
        "external/quan-node/src/Serialization/JsonOutputStreamSerializer.cpp"
        "external/quan-node/src/Serialization/KVBinaryInputStreamSerializer.cpp"
        "external/quan-node/src/Serialization/KVBinaryOutputStreamSerializer.cpp"
        "external/quan-node/src/Serialization/SerializationOverloads.cpp"
        "external/quan-node/src/Transfers/BlockchainSynchronizer.cpp"
        "external/quan-node/src/Transfers/SynchronizationState.cpp"
        "external/quan-node/src/Transfers/TransfersConsumer.cpp"
        "external/quan-node/src/Transfers/TransfersContainer.cpp"
        "external/quan-node/src/Transfers/TransfersSubscription.cpp"
        "external/quan-node/src/Transfers/TransfersSynchronizer.cpp"
        "external/quan-node/src/Wallet/LegacyKeysImporter.cpp"
        "external/quan-node/src/Wallet/WalletAsyncContextCounter.cpp"
        "external/quan-node/src/Wallet/WalletErrors.cpp"
        "external/quan-node/src/Wallet/WalletSerializationV1.cpp"
        "external/quan-node/src/Wallet/WalletSerializationV2.cpp"
        "external/quan-node/src/Wallet/WalletUtils.cpp"
        "external/quan-node/src/WalletLegacy/KeysStorage.cpp"
        "external/quan-node/src/WalletLegacy/WalletLegacy.cpp"
        "external/quan-node/src/WalletLegacy/WalletHelper.cpp"
        "external/quan-node/src/WalletLegacy/WalletLegacySerializer.cpp"
        "external/quan-node/src/WalletLegacy/WalletLegacySerialization.cpp"
        "external/quan-node/src/WalletLegacy/WalletTransactionSender.cpp"
        "external/quan-node/src/WalletLegacy/WalletUnconfirmedTransactions.cpp"
        "external/quan-node/src/WalletLegacy/WalletUserTransactionsCache.cpp"
        "external/quan-node/src/System/ContextGroup.cpp"
        "external/quan-node/src/System/Event.cpp"
        "external/quan-node/src/System/EventLock.cpp"
        "external/quan-node/src/System/InterruptedException.cpp"
        "external/quan-node/src/System/Ipv4Address.cpp"
        "external/quan-node/src/System/TcpStream.cpp"
        "external/quan-node/src/HTTP/HttpRequest.cpp"
        "external/quan-node/src/HTTP/HttpParser.cpp"
        "external/quan-node/src/HTTP/HttpParserErrorCodes.cpp"
        "external/quan-node/src/HTTP/HttpResponse.cpp"
        "external/quan-node/src/Rpc/HttpClient.cpp"
        "external/quan-node/src/Rpc/JsonRpc.cpp"
        "external/quan-node/src/P2p/NetNode.cpp"
        "external/quan-node/src/P2p/LevinProtocol.cpp"
        "external/quan-node/src/P2p/NetNodeConfig.cpp"
        "external/quan-node/src/P2p/PeerListManager.cpp"
        "external/quan-node/src/CryptoNoteProtocol/CryptoNoteProtocolHandler.cpp"
        "external/quan-node/src/Logging/ILogger.cpp"
        "external/quan-node/src/Logging/LoggerGroup.cpp"
        "external/quan-node/src/Logging/CommonLogger.cpp"
        "external/quan-node/src/Logging/LoggerManager.cpp"
        "external/quan-node/src/Logging/FileLogger.cpp"
        "external/quan-node/src/Logging/StreamLogger.cpp"
        "external/quan-node/src/Logging/ConsoleLogger.cpp"
        "external/quan-node/src/Logging/LoggerMessage.cpp"
        "external/quan-node/src/Logging/LoggerRef.cpp"
        "external/quan-node/src/BlockchainExplorer/BlockchainExplorer.cpp"
        "external/quan-node/src/BlockchainExplorer/BlockchainExplorerDataBuilder.cpp"
        "external/quan-node/src/BlockchainExplorer/BlockchainExplorerErrors.cpp")