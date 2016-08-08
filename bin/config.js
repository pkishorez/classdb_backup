{
	"n_collections" : 1,
	"collections" : [
		{
			"offset" : 32,
			"name" : "User",
			"type" : "COLLECTION_F",
			"collection" : {
				"block_size" : 100,
				"offset" : 0,
				"n_properties" : 2,
				"properties" : [
					{
						"name" : "Profile",
						"type" : "PROPERTY_FS",
						"offset" : 0,
						"property" : {
							"offset" : 0,
							"size" : 100,
							"n_datoms" : 5,
							"datoms" : [
								{
									"name": "name",
									"size": 20,
									"offset" : 0,
									"type": "DATOM_STRING"
								},
								{
									"name": "father",
									"size": 20,
									"offset" : 20,
									"type": "DATOM_STRING"
								},
								{
									"name": "mother",
									"size": 20,
									"offset" : 40,
									"type": "DATOM_STRING"
								},
								{
									"name": "college",
									"size": 20,
									"offset" : 60,
									"type": "DATOM_STRING"
								},
								{
									"name": "age",
									"size": -1,
									"offset" : 80,
									"type": "DATOM_INT32"
								}
							]
						}
					},
					{
						"name" : "Posts",
						"type" : "PROPERTY_FM",
						"offset" : 100,
						"property" : {
							"offset" : 0,
							"size" : 100,
							"ref" : "id",
							"prop" : {
								"offset" : 0,
								"size" : 16,
								"n_datoms" : 2,
								"datoms" : [
									{
										"name": "id",
										"size": 0,
										"offset" : 0,
										"type": "DATOM_INT32"
									},
									{
										"name": "time",
										"size": 4,
										"offset" : 8,
										"type": "DATOM_INT32"
									}
								]
							}

						}
					}
				]
			}
		}
	]
}
