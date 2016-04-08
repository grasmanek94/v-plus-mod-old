#pragma once

enum NativeIdentifiers : uint64_t
{
	GET_PLAYER_PED = 0x43A66C31C68491C0,
	GET_ENTITY_COORDS = 0x3FEF770D40960D5A,
	GET_FIRST_BLIP_INFO_ID = 0x1BEDE233E6CD2A1F,
	GET_NEXT_BLIP_INFO_ID = 0x14F96AA50D6FBEA7,
	GET_BLIP_INFO_ID_TYPE = 0xBE9B0959FFD0779B,
	GET_BLIP_COORDS = 0x586AFE3FF72D996E,
	GET_GROUND_Z_FOR_3D_COORD = 0xC906A7DAB05C8D2B,
	SET_ENTITY_COORDS = 0x621873ECE1178967,
	SET_ENTITY_COORDS_NO_OFFSET = 0x239A3351AC1DA385,
	LOAD_SCENE = 0x4448EB75B4904BDB,
	REQUEST_MODEL = 0x963D27A58DF860AC,
	HAS_MODEL_LOADED = 0x98A4EB5D89A0C952,
	CREATE_VEHICLE = 0xAF35D0D2583051B0,
	SHUTDOWN_LOADING_SCREEN = 0x078EBE9809CCD637,
	DO_SCREEN_FADE_IN = 0xD4E8E24955024033,
	NETWORK_IS_HOST = 0x8DB296B814EDDA07,
	NETWORK_RESURRECT_LOCAL_PLAYER = 0xEA23C49EAA83ACFB,
	NETWORK_IS_GAME_IN_PROGRESS = 0x10FAB35428CCC9D7,
	IS_ENTITY_DEAD = 0x5F9532F3B5CC2551,
	SET_NOTIFICATION_TEXT_ENTRY = 0x202709F4C58A0424,
	ADD_TEXT_COMPONENT_STRING = 0x6C188BE134E074AA,
	DRAW_NOTIFICATION = 0x2ED7843F8F801023,
	SET_PED_DENSITY_MULTIPLIER_THIS_FRAME = 0x95E3D6257B166CF2,
	SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME = 0x7A556143A1C03898,
	SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME = 0x245A6883D966D537,
	SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME = 0xEAE6DCC7EEE3DB1D,
	SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME = 0xB3B3359379FE77D3,
	SET_MAX_WANTED_LEVEL = 0xAA5F02DB48D704B9,
	GIVE_DELAYED_WEAPON_TO_PED = 0xB282DC6EBD803C75,
	GET_HASH_KEY = 0xD24D37CC275948CC,
	DOES_ENTITY_EXIST = 0x7239B21A38F536BA,
	SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME = 0x2F9A292AD0A3BD89,
	SUPPRESS_AGITATION_EVENTS_NEXT_FRAME = 0x5F3B7749C112D552,
	SET_GARBAGE_TRUCKS = 0x2AFD795EEAC8D30D,
	SET_RANDOM_BOATS = 0x84436EC293B1415F,
	SET_RANDOM_TRAINS = 0x80D9F74197EA47D9,
	SET_TIME_SCALE = 0x1D408577D440E81E,
	CREATE_PED = 0xD49F9B0955C367DE,
	DELETE_PED = 0x9614299DCB53E54B,
	SET_PED_CAN_BE_TARGETTED = 0x63F58F7C80513AAD,
	IS_ENTITY_A_PED = 0x524AC5ECEA15343E,
	IS_PED_DEAD = 0x3317DEDB88C95038,
	GET_ENTITY_ROTATION = 0xAFBD61CC738D9EB9,
	GET_ENTITY_VELOCITY = 0x4805D2B1D8CF94A9,
	SET_ENTITY_ROTATION = 0x8524A8B0171D5E07,
	SET_ENTITY_VELOCITY = 0x1C99BB7B6E96D16F,
	SET_ENTITY_AS_MISSION_ENTITY = 0xAD738C3085FE7E11,
	DELETE_ENTITY = 0xAE3CBE5BF394C9C9,
	RESURRECT_PED = 0x71BC8E838B9C6035,
	SET_PED_CAN_RAGDOLL = 0xB128377056A54E2A,
};

class TestThread : public ScriptThread
{
private:
	bool m_bInitialized;
	bool m_bIsWaitingForPedModelToLoad;
	bool m_bIsWaitingForVehicleModelToLoad;
	std::list<scrPed> m_spawnedPeds;
	bool m_bIsWaitingForClonePedModelToLoad;
	bool m_bClonePedSpawned;
	scrPed m_clonePed;

public:
	TestThread();

	virtual eScriptThreadState Reset(uint32_t scriptHash, void* pArgs, uint32_t argCount) override;
	virtual void DoRun() override;
};
