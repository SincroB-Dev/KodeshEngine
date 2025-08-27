#include <Editor/Windows/UINodeEditor/Nodes/Node.hpp>
#include <algorithm>
#include <sstream>
#include <string>

namespace editor::nodes
{
 	unsigned int Node::s_NextUIID = 0;

	Socket* Node::AddInput(ine::PinId id, std::string name, SocketType type)
	{
		Inputs.emplace_back(std::make_unique<Socket>(id, name, type));

		Socket* socket = Inputs.back().get();

		socket->NodePtr = this;
        socket->Kind = SocketKind::Input;

		if (OnAddInput)
		{
			OnAddInput(id, socket);
		}

		return socket;
	}
	
	Socket* Node::AddOutput(ine::PinId id, std::string name, SocketType type)
	{
		Outputs.emplace_back(std::make_unique<Socket>(id, name, type));

		Socket* socket = Outputs.back().get();

		socket->NodePtr = this;
        socket->Kind = SocketKind::Output;

		if (OnAddOutput)
		{
			OnAddOutput(id, socket);
		}

		return socket;
	}

	void Node::RemoveInput(ine::PinId id)
	{
		Inputs.erase(
			std::remove_if(
				Inputs.begin(), 
				Inputs.end(), 
				[this, id](const std::unique_ptr<Socket>& socket){
					if (socket->ID == id)
					{
						if (this->OnRemoveInput)
						{
							this->OnRemoveInput(id);
						}
						return true;
					}
					return false;
				} 
			),
			Inputs.end()
		);
	}
	void Node::RemoveOutput(ine::PinId id)
	{
		Inputs.erase(
			std::remove_if(
				Inputs.begin(), 
				Inputs.end(), 
				[this, id](const std::unique_ptr<Socket>& socket){
					if (socket->ID == id)
					{
						if (this->OnRemoveOutput)
						{
							this->OnRemoveOutput(id);
						}
						return true;
					}
					return false;
				}
			),
			Inputs.end()
		);
	}

	bool Node::ValidateType(SocketType type, const SocketValue& v)
	{
		if (type == SocketType::Int)
		{
			return std::holds_alternative<int>(v);
		}

		else if (type == SocketType::Float)
		{
			return std::holds_alternative<float>(v);
		}

		else if (type == SocketType::Bool)
		{
			return std::holds_alternative<float>(v);
		}

		else if (type == SocketType::String)
		{
			return std::holds_alternative<std::string>(v);
		}

		else if (type == SocketType::Object)
		{
			return std::holds_alternative<std::any>(v);
		}

		return false;
	}

	SocketValue* Node::AddValue(std::string key, SocketType type, SocketValue value)
	{
		if (!ValidateType(type, value))
		{
			throw std::runtime_error("Type mismatch in Node::AddValue");
		}

		// Evita trabalho manual de incrementar nomes.
		std::ostringstream oss;
		oss << key << "###N" << static_cast<int>(ID.Get()) << "C" << s_NextUIID++;

		DataSet.emplace(oss.str(), std::make_unique<NodeValue>(type, value));
		return &DataSet[oss.str()]->Value;
	}

	NodeValue* Node::GetValue(std::string key)
	{
		auto it = DataSet.find(key);
		if (it != DataSet.end())
		{
			return it->second.get();
		}
		return nullptr;
	}
		
	bool Node::LinkValueToOutput(std::string key, Socket* socket)
	{
		return false;
	}

	bool Node::LinkValueToInput(std::string key, Socket* socket)
	{
		return false;
	}
}