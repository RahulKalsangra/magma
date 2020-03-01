#!/usr/bin/env python3
# @generated AUTOGENERATED file. Do not Change!

from dataclasses import dataclass
from datetime import datetime
from functools import partial
from gql.gql.datetime_utils import DATETIME_FIELD
from numbers import Number
from typing import Any, Callable, List, Mapping, Optional

from dataclasses_json import dataclass_json

from gql.gql.enum_utils import enum_field
from .service_endpoint_role_enum import ServiceEndpointRole

@dataclass_json
@dataclass
class AddServiceEndpointInput:
    id: str
    portId: str
    role: ServiceEndpointRole = enum_field(ServiceEndpointRole)

