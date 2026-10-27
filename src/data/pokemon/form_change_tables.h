static const struct FormChange sDeoxysNormalFormChangeTable[] =
{
    {FORM_CHANGE_ITEM_USE, SPECIES_DEOXYS_ATTACK, ITEM_METEORITE, 0},
    {FORM_CHANGE_TERMINATOR, SPECIES_NONE, ITEM_NONE, 0},
};

static const struct FormChange sDeoxysAttackFormChangeTable[] =
{
    {FORM_CHANGE_ITEM_USE, SPECIES_DEOXYS_DEFENSE, ITEM_METEORITE, 0},
    {FORM_CHANGE_TERMINATOR, SPECIES_NONE, ITEM_NONE, 0},
};

static const struct FormChange sDeoxysDefenseFormChangeTable[] =
{
    {FORM_CHANGE_ITEM_USE, SPECIES_DEOXYS_SPEED, ITEM_METEORITE, 0},
    {FORM_CHANGE_TERMINATOR, SPECIES_NONE, ITEM_NONE, 0},
};

static const struct FormChange sDeoxysSpeedFormChangeTable[] =
{
    {FORM_CHANGE_ITEM_USE, SPECIES_DEOXYS, ITEM_METEORITE, 0},
    {FORM_CHANGE_TERMINATOR, SPECIES_NONE, ITEM_NONE, 0},
};

static const struct FormChange sRotomFormChangeTable[] =
{
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM, ITEM_ROTOM_CATALOG, 0},
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM_HEAT, ITEM_ROTOM_CATALOG, 1},
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM_WASH, ITEM_ROTOM_CATALOG, 2},
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM_FROST, ITEM_ROTOM_CATALOG, 3},
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM_FAN, ITEM_ROTOM_CATALOG, 4},
    {FORM_CHANGE_ITEM_USE_MULTICHOICE, SPECIES_ROTOM_MOW, ITEM_ROTOM_CATALOG, 5},
    {FORM_CHANGE_TERMINATOR, SPECIES_NONE, ITEM_NONE, 0},
};

static const struct
{
    u16 species;
    const struct FormChange *table;
} sSpeciesFormChangeTablePointers[] =
{
    {SPECIES_DEOXYS, sDeoxysNormalFormChangeTable},
    {SPECIES_DEOXYS_ATTACK, sDeoxysAttackFormChangeTable},
    {SPECIES_DEOXYS_DEFENSE, sDeoxysDefenseFormChangeTable},
    {SPECIES_DEOXYS_SPEED, sDeoxysSpeedFormChangeTable},
    {SPECIES_ROTOM, sRotomFormChangeTable},
    {SPECIES_ROTOM_HEAT, sRotomFormChangeTable},
    {SPECIES_ROTOM_WASH, sRotomFormChangeTable},
    {SPECIES_ROTOM_FROST, sRotomFormChangeTable},
    {SPECIES_ROTOM_FAN, sRotomFormChangeTable},
    {SPECIES_ROTOM_MOW, sRotomFormChangeTable},
};
