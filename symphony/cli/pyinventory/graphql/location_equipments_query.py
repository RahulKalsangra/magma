#!/usr/bin/env python3
# @generated AUTOGENERATED file. Do not Change!

from dataclasses import dataclass
from datetime import datetime
from gql.gql.datetime_utils import DATETIME_FIELD
from gql.gql.graphql_client import GraphqlClient
from functools import partial
from numbers import Number
from typing import Any, Callable, List, Mapping, Optional

from dataclasses_json import dataclass_json


@dataclass_json
@dataclass
class LocationEquipmentsQuery:
    __QUERY__ = """
    query LocationEquipmentsQuery($id: ID!) {
  location: node(id: $id) {
    ... on Location {
      equipments {
        id
        name
        equipmentType {
          name
        }
      }
    }
  }
}

    """

    @dataclass_json
    @dataclass
    class LocationEquipmentsQueryData:
        @dataclass_json
        @dataclass
        class Node:
            @dataclass_json
            @dataclass
            class Equipment:
                @dataclass_json
                @dataclass
                class EquipmentType:
                    name: str

                id: str
                name: str
                equipmentType: EquipmentType

            equipments: List[Equipment]

        location: Optional[Node] = None

    data: Optional[LocationEquipmentsQueryData] = None
    errors: Optional[Any] = None

    @classmethod
    # fmt: off
    def execute(cls, client: GraphqlClient, id: str):
        # fmt: off
        variables = {"id": id}
        response_text = client.call(cls.__QUERY__, variables=variables)
        return cls.from_json(response_text).data
