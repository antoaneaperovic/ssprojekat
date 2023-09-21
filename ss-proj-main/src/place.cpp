#include "place.hpp"

regex Place::REGEX_PLACE = regex("^-place=([a-zA-Z_][a-zA-Z_0-9]*)@(0[xX][0-9a-fA-F]+)$");

Place::Place(string place)
{
    smatch smatchPlace;
    if (regex_search(place, smatchPlace, REGEX_PLACE))
    {
        sectionName = smatchPlace.str(1);
        offset = convert_hex_to_unsigned_decimal(smatchPlace.str(2));
    }
    else {
        throw Exception("Problem with place." + place, 3, -1, place);
    }
}
