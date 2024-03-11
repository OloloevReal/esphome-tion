import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button, switch
from esphome.const import (
    CONF_ENTITY_CATEGORY,
    CONF_ICON,
    CONF_ID,
    CONF_TYPE,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import (
    CONF_TION_COMPONENT_CLASS,
    TionResetFilterConfirmSwitchT,
    get_pc_info,
    new_pc_component,
    pc_schema,
    setup_switch,
    tion_ns,
)

AUTO_LOAD = ["switch"]

CONF_RESET_FILTER_CONFIRM = "confirm"

TionButton = tion_ns.class_("TionButton", button.Button, cg.Component)

PROPERTIES = {
    "reset_filter": {
        CONF_TION_COMPONENT_CLASS: tion_ns.class_(
            "TionResetFilterButton2", button.Button, cg.Component
        ),
        CONF_ICON: "mdi:wrench-cog",
        CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_CONFIG,
    },
    # "reset_errors": {
    #     CONF_ICON: "mdi:button-pointer",
    #     CONF_ENTITY_CATEGORY: ENTITY_CATEGORY_DIAGNOSTIC,
    # },
}


def check_type(key, typ):
    def validator(config):
        if key in config and config[CONF_TYPE] != typ:
            raise cv.Invalid(f"{key} is not valid for the type {typ}")
        return config

    return validator


CONFIG_SCHEMA = cv.All(
    pc_schema(
        button.button_schema(TionButton).extend(
            {
                cv.Optional(CONF_RESET_FILTER_CONFIRM): switch.switch_schema(
                    TionResetFilterConfirmSwitchT.template(cg.Component),
                    icon="mdi:wrench-check",
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
            }
        ),
        PROPERTIES,
    ),
    check_type(CONF_RESET_FILTER_CONFIRM, "reset_filter"),
)


async def button_new_button(config, *args):
    (_, props) = get_pc_info(config, PROPERTIES)
    if CONF_TION_COMPONENT_CLASS in props:
        # hacky inject CONF_TION_COMPONENT_CLASS into CONF_ID
        config[CONF_ID].type = props[CONF_TION_COMPONENT_CLASS]
    return await button.new_button(config, *args)


async def to_code(config: dict):
    var = await new_pc_component(config, button_new_button, PROPERTIES)
    await setup_switch(config, CONF_RESET_FILTER_CONFIRM, var.set_confirm, var)
