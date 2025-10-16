# MEOW: Man eat omelette washed
a minimal (?) compiler (??) that compile to scratch.

## the scratch project.json format
refer to https://github.com/scratchfoundation/scratch-parser/blob/0f3a10d9cf363c410b924aaeff1ea6a7a346f6a1/lib/sb3_definitions.json

### opcodes
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

### declaring variable
```json
"variables": {
    "type": "object",
    "additionalProperties": {"$ref":"#/definitions/scalar_variable"}
},
"scalar_variable": {
    "type": "array",
    "items": [
        {"type": "string", "description": "name of the variable"},
        {"$ref":"#/definitions/scalarVal", "description": "value of the variable"}
    ],
    "additionalItems": {"type": "boolean", "enum": [true], "description": "Whether this is a cloud variable"},
    "maxItems": 3
}
```
we don't really care about cloud variable so we can just skip that. we do care about scalarval though.

```json
"stringOrNumber": {
    "oneOf": [
        {"type": "string"},
        {"type": "number"}
    ]
},
"scalarVal": {
    "oneOf": [
        {"$ref":"#/definitions/stringOrNumber"},
        {"type": "boolean"}
    ]
}
```

the end result is:
```json
"variables": {
    "randomId": ["actual variable name", 12]
}
```

## Scratch blocks
you can look at each blocks parameter in https://github.com/scratchfoundation/scratch-vm/tree/b3266a0cfe5122f20b72ccd738a3dd4dff4fc5a5/src/blocks
there should be `getPrimitives()` function that maps opcode to function in the vm itself, and you can see the arguments used in each function.
for example (`looks_say`)
```javascript
getPrimitives () {
    return {
        looks_say: this.say,
        looks_sayforsecs: this.sayforsecs,
        // ...
    }
}

// ...

say (args, util) {
    // @TODO in 2.0 calling say/think resets the right/left bias of the bubble
    this.runtime.emit(Scratch3LooksBlocks.SAY_OR_THINK, util.target, 'say', args.MESSAGE);
}
```

so, the resulting block should be something like this:
```json
{
  "opcode": "looks_say",
  "topLevel": true,
  "next": null,
  "inputs": {
    "MESSAGE": [
      1,
      [
        10,
        "yo"
      ]
    ]
  }
}
```

Also, there are blocks with fields (eg. `looks_changeeffectby`) that allows choosing type from selected category.
```javascript
// this.changeEffect
changeEffect (args, util) {
    const effect = Cast.toString(args.EFFECT).toLowerCase();
    const change = Cast.toNumber(args.CHANGE);
    if (!Object.prototype.hasOwnProperty.call(util.target.effects, effect)) return;
    let newValue = change + util.target.effects[effect];
    newValue = this.clampEffect(effect, newValue);
    util.target.setEffect(effect, newValue);
}
```
you can see that it refers to EFFECT as args, yet the generated blocks from Scratch 3 looks like this (some field omitted for readability):
```json
{
  "opcode": "looks_changeeffectby",
  "inputs": {
    "CHANGE": [
      1,
      [
        4,
        "25"
      ]
    ]
  },
  "fields": {
    "EFFECT": [
      "COLOR",
      null
    ]
  },
}
```
it is in fields!! and scratch-parser don't have proper type for fields except for it being a `object`.
But!! we can look at https://github.com/scratchfoundation/scratch-editor/blob/aa654093ab5e42307f3b14f6aac8a6a3783b1612/packages/scratch-vm/src/serialization/sb3.js and see the implementation.
```javascript
const serializeFields = function (fields) {
    const obj = Object.create(null);
    for (const fieldName in fields) {
        if (!hasOwnProperty.call(fields, fieldName)) continue;
        obj[fieldName] = [fields[fieldName].value];
        if (Object.prototype.hasOwnProperty.call(fields[fieldName], 'id')) {
            obj[fieldName].push(fields[fieldName].id);
        }
    }
    return obj;
};
```
fields are currently not planned to be used in this project (unless I plan to port all scratch opcode as a api)
