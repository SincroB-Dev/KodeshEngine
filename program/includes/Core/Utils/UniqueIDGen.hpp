#pragma once

#include <vector>
#include <atomic>
#include <queue>

namespace core
{
	namespace utils
	{
		struct UniqueID
		{
			uint32_t ID;
			uint32_t Generation;

			UniqueID(): ID(-1), Generation(-1) {}
			UniqueID(uint32_t id): ID(id), Generation(0) {}
			UniqueID(uint32_t id, uint32_t gen): ID(id), Generation(gen) {}

			inline bool operator == (const UniqueID& other) const
			{
				return ID == other.ID && Generation == other.Generation;
			}
		};

		class UniqueIDGen
		{
		public:
			UniqueIDGen(): m_NextID(0) {};

			// Cria um novo objeto, reutilizando o ID se possível
			UniqueID CreateUniqueID();

			// Marca o objeto como destruído e libera o ID
			bool DestroyID(const UniqueID& unique);

			// Verifica se o id ainda é válido
			bool IsAlive(const UniqueID& unique) const;

		    // método de cópia manual
		    UniqueIDGen GetCopy() const;

		private:
			// Controle de geração de identificador único
			std::atomic<uint32_t> m_NextID;
			std::vector<uint32_t> m_Generations;
			std::queue<uint32_t> m_FreeList;

		private:
			// construtor de inicialização manual para clone
		    UniqueIDGen(uint32_t nextID, const std::vector<uint32_t>& generations,
		                const std::queue<uint32_t>& freeList)
		        : m_NextID(nextID)
		        , m_Generations(generations)
		        , m_FreeList(freeList)
		    {}
		};
	}
}