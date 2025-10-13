# MEOW: Man eat omelette washed
a minimal (?) compiler (??) that compile to scratch.

## the scratch instruction
refer to https://github.com/scratchfoundation/scratch-parser/blob/0f3a10d9cf363c410b924aaeff1ea6a7a346f6a1/lib/sb3_definitions.json
```json
{
  "num_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [4, 5, 6, 7, 8]
      },
      {
        "$ref": "#/definitions/stringOrNumber"
      }
    ],
    "additionalItems": false
  },
  "color_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [9]
      },
      {
        "type": "string",
        "pattern": "^#[a-fA-F0-9]{6}$"
      }
    ],
    "additionalItems": false
  },
  "text_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [10]
      },
      {
        "$ref": "#/definitions/stringOrNumber"
      }
    ],
    "additionalItems": false
  },
  "broadcast_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [11]
      },
      {
        "type": "string",
        "description": "broadcast message"
      },
      {
        "type": "string",
        "description": "broadcast message id"
      }
    ],
    "additionalItems": false
  },
  "variable_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [12]
      },
      {
        "type": "string",
        "description": "variable name"
      },
      {
        "type": "string",
        "description": "variable id"
      }
    ],
    "additionalItems": {
      "type": "number"
    },
    "minItems": 3,
    "maxItems": 5
  },
  "list_primitive": {
    "type": "array",
    "items": [
      {
        "type": "number",
        "enum": [13]
      },
      {
        "type": "string",
        "description": "list name"
      },
      {
        "type": "string",
        "description": "list id"
      }
    ],
    "additionalItems": {
      "type": "number"
    },
    "minItems": 3,
    "maxItems": 5
  }
}
```

when we do block and need inputs, the schema is like this:
```json
"inputs": {
    "type": "object",
    "additionalProperties": {
        "type": "array",
        "items": [
            {
                "type":"number",
                "enum":[1,2,3],
                "description": "1 = unobscured shadow, 2 = no shadow, 3 = obscured shadow"
            }
        ],
        "additionalItems": {
            "oneOf": [
                {"$ref":"#/definitions/optionalString"},
                {"$ref":"#/definitions/inputPrimitive"}
            ]
        }
    }
}
```

so, we can do (maybe) something like:
```json
[2, [4, "16"]]
```
