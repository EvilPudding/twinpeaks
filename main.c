#include <candle.h>
#include <stdio.h>
#include <components/name.h>
#include <components/decal.h>
#include <components/sprite.h>
#include <components/light.h>
#include <components/model.h>
#include <systems/editmode.h>

int main(int argc, char **argv)
{
	candle_init(argv[0]);

	entity_add_component(SYS, (c_t*)c_editmode_new());
	c_editmode_activate(c_editmode(&SYS));

	c_spatial_t *cam = c_spatial(&c_editmode(&SYS)->camera);
	c_spatial_set_pos(cam, vec3(0.0f, 3.0f, 6.0f));
	c_spatial_rotate_Y(cam, -M_PI / 2.0f);

	c_sauces_index_dir(c_sauces(&SYS), "resauces");

	mesh_t *floor = sauces("floor");
	mesh_t *cloth = sauces("cloth");
	cloth->smooth_angle = 1.0f;

	entity_new(c_model_new(floor, sauces("tile"), 1, 1));

	entity_t c0 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c1 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c2 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c3 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));

	mat1f(sauces("tile"), ref("normal.blend"), 0.3f);

	c_spatial_rotate_Y(c_spatial(&c1), M_PI / 2);
	c_spatial_rotate_Y(c_spatial(&c2), M_PI);
	c_spatial_rotate_Y(c_spatial(&c3), 3 * M_PI / 2);

	entity_t light = entity_new({
		c_name_new("light");
		c_light_new(30.0f, vec4(0.9, 0.9, 0.8, 1.0));
	});
	c_spatial_set_pos(c_spatial(&light), vec3(2.0, 8.0, 2.0));

	entity_t ambient = entity_new({
		c_name_new("light");
		c_light_new(-1.0f, vec4(0.1, 0.1, 0.2, 1.0));
	});
	c_spatial_set_pos(c_spatial(&ambient), vec3(2.0, 10.0, 2.0));

	mat_t *stone = mat_new("stone", "default");
	mat4f(stone, ref("albedo.color"), vec4(0.9, 0.9, 0.9, 1.0));
	mat1f(stone, ref("roughness.value"), 0.65f);
	mat1t(stone, ref("roughness.texture"), sauces("rough.png"));
	mat1f(stone, ref("roughness.blend"), 0.9f);
	mat1f(stone, ref("metalness.value"), 0.95f);
	entity_t venus = entity_new({
		c_name_new("venus");
		c_model_new(sauces("venus.obj"), stone, true, true);
	});
	c_spatial_scale(c_spatial(&venus), vec3(1.3f, 1.3f, 1.3));

	candle_wait();

    return 0;
}
//*/
