#include "../thirdparty/sc/map/sc_map.h"
#include "../include/c_test.h"

#include "../include/log.h"
#include <stdio.h>

void example_str(void)
{
	const char *key, *value;
	struct sc_map_str map;

	sc_map_init_str(&map, 0, 0);

	key = "fuck";
	value = sc_map_get_str(&map, key);
	if(sc_map_found(&map)) {
		INFOF("Key:[%s], Value:[%s]", key, value);
	} else {
		ERRORF("Key:[%s] not found", key);
	}

	sc_map_put_str(&map, "jack", "chicago");
	// sc_map_put_str(&map, "jane", "new york");
	// sc_map_put_str(&map, "janie", "atlanta");

	sc_map_foreach (&map, key, value) {
		INFOF("Key:[%s], Value:[%s]", key, value);
	}

	sc_map_term_str(&map);
}

void example_int_to_str(void)
{
	uint32_t key;
	const char *value;
	struct sc_map_64s map;

	sc_map_init_64s(&map, 0, 0);

	sc_map_put_64s(&map, 100, "chicago");
	sc_map_put_64s(&map, 200, "new york");
	sc_map_put_64s(&map, 300, "atlanta");

	value = sc_map_get_64s(&map, 200);
	if (sc_map_found(&map)) {
		INFOF("Found Value:[%s]", value);
	}

	value = sc_map_del_64s(&map, 100);
	if (sc_map_found(&map)) {
		INFOF("Deleted : %s", value);
	}

	sc_map_foreach (&map, key, value) {
		INFOF("Key:[%d], Value:[%s]", key, value);
	}

	value = sc_map_del_64s(&map, 200);
	if (sc_map_found(&map)) {
		INFOF("Found : %s", value);
	}

	value = sc_map_put_64s(&map, 300, "los angeles");
	if (sc_map_found(&map)) {
		INFOF("overridden : %s", value);
	}

	sc_map_term_64s(&map);
}

void test_map_example(void)
{
	INFO("test func example_str");
	example_str();
	INFO("test func example_int_to_str");
	example_int_to_str();

}
