import json
import argparse
import os
from pathlib import Path
from string import Template

from astarte.device import Interface

mapping_reliability_lookup = {
    0: "RELIABILITY_UNRELIABLE",
    1: "RELIABILITY_GUARANTEED",
    2: "RELIABILITY_UNIQUE",
}

interface_header_template = Template(
    r"""/*
 * (C) Copyright 2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_interface_gen.h
 * @brief Contains automatically generated interfaces.
 *
 * @details The generated structures contain all information regarding each interface.
 * and are automatically generated from the json interfaces definitions.
 */

#ifndef _ASTARTE_INTERFACE_GEN_H_
#define _ASTARTE_INTERFACE_GEN_H_

#include "astarte_interface.h"

${interfaces_declarations}

#endif /* _ASTARTE_INTERFACE_GEN_H_ */
"""
)

interface_source_template = Template(
    r"""/*
 * (C) Copyright 2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_interface_gen.c
 * @brief Contains automatically generated interfaces.
 */

#include "astarte_interface_gen.h"
${interfaces_declarations}
"""
)

interface_declaration_template = Template(
    r"""extern const astarte_interface_t astarte_interface_${interface_name_sc};"""
)

interface_definition_template = Template(
    r"""
static const astarte_mapping_t astarte_mappings_${interface_name_sc}[${mappings_number}] = {${mappings}
};

const astarte_interface_t astarte_interface_${interface_name_sc} = {
    .name = "${interface_name}",
    .major_version = ${version_major},
    .minor_version = ${version_minor},
    .type = ${type},
    .ownership = ${ownership},
    .aggregation = ${aggregation},
    .mappings = astarte_mappings_${interface_name_sc},
};"""
)

mapping_definition_template = Template(
    r"""
    {
        .endpoint = "${endpoint}",
        .type = ${type},
        .reliability = ${reliability},
        .explicit_timestamp = ${explicit_timestamp},
        .allow_unset =  ${allow_unset},
    },"""
)


def generate_interfaces(interfaces_dir: Path, header_dir: Path, source_dir: Path):
    """
    Generate the C files defining a set of interfaces starting from .json definitions.

    Parameters
    ----------
    interfaces_dir : Path
        Folder in which to search for .json files.
    header_dir : Path
        Folder where the generated header fill will be saved.
    source_dir : Path
        Folder where the generated source fill will be saved.
    """

    # Iterate over all the interfaces
    interfaces_declarations = []
    interfaces_structs = []
    for interface_file in [i for i in interfaces_dir.iterdir() if i.suffix == ".json"]:
        with open(interface_file, "r", encoding="utf-8") as interface_fp:
            interface_json = json.load(interface_fp)
            interface = Interface(interface_json)

            # Iterate over each mapping
            mappings_struct = []
            for mapping in interface.mappings:
                # Fill in the mapping information in the template
                mapping_struct = mapping_definition_template.substitute(
                    endpoint=mapping.endpoint,
                    type="TYPE_" + mapping.type.upper(),
                    reliability=mapping_reliability_lookup[mapping.reliability],
                    explicit_timestamp="true" if mapping.explicit_timestamp else "false",
                    allow_unset="true" if mapping.allow_unset else "false",
                )
                mappings_struct.append(mapping_struct)

            # Fill in the interface information in the template
            itype = "TYPE_PROPERTIES" if interface.is_type_properties() else "TYPE_DATASTREAM"
            iownership = "OWNERSHIP_SERVER" if interface.is_server_owned() else "OWNERSHIP_DEVICE"
            iaggregation = (
                "AGGREGATION_OBJECT"
                if interface.is_aggregation_object()
                else "AGGREGATION_INDIVIDUAL"
            )
            interface_struct = interface_definition_template.substitute(
                mappings_number=len(interface.mappings),
                interface_name_sc=interface.name.replace(".", "_"),
                interface_name=interface.name,
                version_major=interface.version_major,
                version_minor=interface.version_minor,
                type=itype,
                ownership=iownership,
                aggregation=iaggregation,
                mappings="".join(mappings_struct),
            )
            interfaces_structs.append(interface_struct)

            # Fill in the extern definition
            interface_declaration = interface_declaration_template.substitute(
                interface_name_sc=interface.name.replace(".", "_")
            )
            interfaces_declarations.append(interface_declaration)

    # Fill in the header
    interfaces_header = interface_header_template.substitute(
        interfaces_declarations="\n".join(interfaces_declarations)
    )

    interfaces_source = interface_source_template.substitute(
        interfaces_declarations="\n".join(interfaces_structs)
    )

    generated_header = header_dir.joinpath("astarte_interface_gen.h")
    with open(generated_header, "w", encoding="utf-8") as generated_fp:
        generated_fp.write(interfaces_header)

    generated_source = source_dir.joinpath("astarte_interface_gen.c")
    with open(generated_source, "w", encoding="utf-8") as generated_fp:
        generated_fp.write(interfaces_source)


# When running as a standalone script
if __name__ == "__main__":
    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description="Parse the warnings.txt generated by the ESP IDF clang-check."
    )
    parser.add_argument(
        "interfaces_dir",
        nargs="?",
        default=Path(os.getcwd())
        .joinpath("examples")
        .joinpath("aggregates")
        .joinpath("interfaces"),
        help="Location for the interfaces .json file(s).",
    )
    parser.add_argument(
        "-i",
        "--header-dir",
        dest="header_dir",
        default=Path(os.getcwd())
        .joinpath("examples")
        .joinpath("aggregates")
        .joinpath("main")
        .joinpath("include"),
        help="Output directory where the generated header file will be placed.",
    )
    parser.add_argument(
        "-s",
        "--source-dir",
        dest="source_dir",
        default=Path(os.getcwd())
        .joinpath("examples")
        .joinpath("aggregates")
        .joinpath("main")
        .joinpath("src"),
        help="Output directory where the generated source file will be placed.",
    )

    args = parser.parse_args()

    generate_interfaces(args.interfaces_dir, args.header_dir, args.source_dir)
