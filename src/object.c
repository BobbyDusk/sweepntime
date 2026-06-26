#include "object.h"
#include "components.h"

ecs_entity_t CreateObject(ecs_world_t *ecs_world, float x, float y, float width, float height,
                          bool movable, SDL_Color color) {
  ecs_entity_t e = ecs_new(ecs_world);

  float half_width = (width / 2.0F);
  float half_height = (height / 2.0F);

  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.type = (int)movable ? b2_dynamicBody : b2_staticBody;
  bodyDef.position = (b2Vec2){x, y};

  // Crucial for Top-Down games: simulate floor friction using linear damping
  if (movable) {
    bodyDef.linearDamping = 3.5F;
  }

  ECS_COMPONENT(ecs_world, PhysicsWorld);
  b2WorldId b2_world = ecs_singleton_get(ecs_world, PhysicsWorld)->world_id;
  b2BodyId bodyId = b2CreateBody(b2_world, &bodyDef);

  b2Polygon boxShape = b2MakeBox(half_width, half_height);
  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = 1.0F; // kg per m^2
  b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

  ECS_COMPONENT(ecs_world, Position);
  ECS_COMPONENT(ecs_world, RigidBody);
  ECS_COMPONENT(ecs_world, Visualization);
  ecs_set(ecs_world, e, Position, {x, y});
  ecs_set(ecs_world, e, RigidBody, {bodyId, half_width, half_height});
  ecs_set(ecs_world, e, Visualization, {color});

  return e;
}
