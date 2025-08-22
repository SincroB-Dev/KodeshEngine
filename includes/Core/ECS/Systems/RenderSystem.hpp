#pragma once

#include "Core/ECS/Systems/ISystem.hpp"

#include "Core/ECS/TransformComponent.hpp"
#include "Core/ECS/ShapeComponent.hpp"

#include "Core/Renderer/RenderCommand.hpp"
#include "Core/Renderer/RenderQueue.hpp"
#include "Core/Renderer/Renderer.hpp"

#include <iostream>

namespace core
{
	namespace ecs
	{
		class RenderSystem : public ISystem
		{
		public:
			inline RenderSystem(renderer::RenderQueue& queue) 
				: queue(queue) {}

			inline void Update(EntityRegistry& registry, double deltaTime) override
			{
				for (auto& e : registry.GetEntities())
				{
					TransformComponent* transform = registry.GetComponent<TransformComponent>(e);
					ShapeComponent* shape = registry.GetComponent<ShapeComponent>(e);

					if (transform && shape)
					{
						renderer::RenderCommand cmd{};

						cmd.vertices = shape->vertices;
						cmd.color = {
							shape->color.r, shape->color.g, shape->color.b, shape->color.a
						};

						cmd.textureID = shape->textureID;

						cmd.x = transform->position.x; cmd.y = transform->position.y;
						cmd.sx = transform->scale.x; cmd.sy = transform->scale.y;

						// orderZ
						cmd.z = shape->orderZ;

						cmd.primitive = shape->primitive;

						queue.Submit(cmd);
					}
				}
			}

		private:
			renderer::RenderQueue& queue; // Fila de renderização;
		};
	}
}