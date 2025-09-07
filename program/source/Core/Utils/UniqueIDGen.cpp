#include "Core/Utils/UniqueIDGen.hpp"

namespace core
{
	namespace utils
	{
		// Cria uma nova entidade, reutilizando o ID se possível
		UniqueID UniqueIDGen::CreateUniqueID()
		{
			uint32_t id;

			if (!m_FreeList.empty())
			{
				// Reaproveita um ID antigo
				id = m_FreeList.front();
				m_FreeList.pop();
			}
			else
			{
				// Cria um novo ID
				id = m_NextID.fetch_add(1, std::memory_order_relaxed);
				m_Generations.push_back(0);
			}

			return UniqueID(id, m_Generations[id]);
		}

		// Retorna o ultimo ID gerado.
		uint32_t UniqueIDGen::LastGeneratedID() const
		{
			return m_NextID.load();
		}

		// Marca a geração como destruída e libera o ID
		bool UniqueIDGen::DestroyID(const UniqueID& unique)
		{
			bool alive = IsAlive(unique);

			if (alive)
			{
				m_Generations[unique.ID]++; // Incrementa a geração
				m_FreeList.push(unique.ID); // ID pode ser reutilizado
			}

			return alive;
		}

		// Verifica se o id ainda é válido
		bool UniqueIDGen::IsAlive(const UniqueID& unique) const
		{
			return unique.ID < m_Generations.size() && m_Generations[unique.ID] == unique.Generation;
		}

		// Pega uma cópia do gerador, geralmente utilizado para troca de modos.
		UniqueIDGen UniqueIDGen::GetCopy() const
		{
	        return UniqueIDGen(m_NextID.load(), m_Generations, m_FreeList);
	    }

	    // Consulta quantas gerações um ID já teve.
	    const std::vector<uint32_t>& UniqueIDGen::GetGenerations() const
	    {
	    	return m_Generations;
	    }

	    // Consulta fila de IDs liberados.
	    std::queue<uint32_t> UniqueIDGen::GetFreeList() const
	    {
	    	return m_FreeList;
	    }
	}
}