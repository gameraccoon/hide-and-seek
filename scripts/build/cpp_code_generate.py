#!/usr/bin/python3

import json
import os
from os import path

import sys

if len(sys.argv) > 1:
    working_dir = sys.argv[1]
else:
    working_dir = os.getcwd()

templates_dir = path.join(working_dir, "config/code_generation/templates")
descriptions_dir = path.join(working_dir, "config/class_descriptions/Components")
configs_dir = path.join(working_dir, "config/code_generation")


def capitalize(input_str):
    return input_str[:1].capitalize() + input_str[1:]


def read_template(template_name):
    with open(path.join(templates_dir, template_name + ".template"), 'r') as template_file:
        return template_file.read().rstrip('\n')


def replace_content(template_string, data_dictionary):
    return template_string.format(**data_dictionary)


def write_file(generated_file_name, content):
    if os.path.isfile(generated_file_name):
        with open(generated_file_name, 'r') as generated_file:
            if content == generated_file.read():
                return

    with open(generated_file_name, 'w') as generated_file:
        generated_file.write(content)


def get_base_data_dictionary(data_description):
    return {
        "component_name": data_description["component"],
        "component_name_capital": capitalize(data_description["component"]),
        "class_name": "%sComponent" % capitalize(data_description["component"]),
        "component_description": data_description["description"]
    }


def get_attribute_data_dictionary(attribute):
    attribute_data_dictionary = {}

    for field_name, field_value in attribute.items():
        attribute_data_dictionary["attribute_" + field_name] = field_value

    attribute_data_dictionary["attribute_name_capital"] = capitalize(attribute["name"])

    if "include" in attribute and len(attribute["include"]) > 0:
        attribute_data_dictionary["attribute_include_full"] = "#include " + attribute["include"]
    else:
        attribute_data_dictionary["attribute_include_full"] = ""

# fill missing fields from defaults
    for field_name, field_value in attribute_optional_fields.items():
        if ("attribute_" + field_name) not in attribute_data_dictionary:
            attribute_data_dictionary["attribute_" + field_name] = field_value

    return attribute_data_dictionary


def append_attributes_data_dictionary(data_dictionary, data_description):
    for attribute_template_data in attribute_templates:
        template_name = attribute_template_data["name"]

        template_params = set(attribute_template_data["params"])

        template = read_template(template_name)
        replacement_content = ""
        replacement_content_elements = []

        # filter attributes
        for attribute in data_description["attributes"]:

            # skip attributes with empty values, if requested
            if "not_empty" in template_params:
                if len(attribute["data_dict"][attribute_template_data["value_to_empty_test"]]) == 0:
                    continue

            # skip blacklisted attributed
            if "blacklist" in attribute_template_data:
                if any(x in attribute["data_dict"]["attribute_flags"] for x in attribute_template_data["blacklist"]):
                    continue

            # if we have whitelist, skip attributes without whitelisted flags
            if "whitelist" in attribute_template_data:
                if not any(x in attribute["data_dict"]["attribute_flags"] for x in attribute_template_data["whitelist"]):
                    continue

            replacement_content_elements.append(attribute["data_dict"])

        # generate content
        for replace_content_dict in replacement_content_elements:
            # skip delimiters for the last attribute
            if replace_content_dict is replacement_content_elements[len(replacement_content_elements) - 1]:
                delimiter_dict = empty_delimiter_dictionary
            else:
                delimiter_dict = delimiter_dictionary

            new_replacement_line = replace_content(template, {
                **data_dictionary,
                **replace_content_dict,
                **delimiter_dict
            })

            replacement_content += new_replacement_line

        if "remove_duplicates" in template_params:
            replacement_content = "\n".join(set(replacement_content.split("\n")))

        if "sort" in template_params:
            replacement_content = "\n".join(sorted(replacement_content.split("\n")))

        data_dictionary[template_name] = replacement_content
    return data_dictionary


def get_full_data_dictionary(data_description):
    full_data_dictionary = get_base_data_dictionary(data_description)
    full_data_dictionary = append_attributes_data_dictionary(full_data_dictionary, data_description)
    return full_data_dictionary


def generate_component_cpp_file(template_name, destination_dir, file_name_template, full_data_dictionary):
    template = read_template(template_name)
    generated_content = replace_content(template, full_data_dictionary)
    file_name = replace_content(file_name_template, full_data_dictionary)

    if not os.path.exists(destination_dir):
        os.makedirs(destination_dir)

    write_file(path.join(destination_dir, file_name), generated_content)


def generate_per_attribute_cpp_files(data_description, template_name, destination_dir, file_name_template, full_data_dictionary):
    template = read_template(template_name)
    for attribute in data_description["attributes"]:
        attribute_dict = {
            **full_data_dictionary,
            **attribute["data_dict"]
        }

        generated_content = replace_content(template, attribute_dict)
        file_name = replace_content(file_name_template, attribute_dict)

        if not os.path.exists(destination_dir):
            os.makedirs(destination_dir)

        write_file(path.join(destination_dir, file_name), generated_content)


def generate_files(file_infos, data_description):
    full_data_dict = get_full_data_dictionary(data_description)
    for file_info in file_infos:
        if "per_attribute" in file_info and file_info["per_attribute"]:
            generate_per_attribute_cpp_files(data_description, file_info["template"],
                                             path.join(working_dir, file_info["output_dir"]),
                                             file_info["name_template"],
                                             full_data_dict)
        else:
            generate_component_cpp_file(file_info["template"],
                                        path.join(working_dir, file_info["output_dir"]),
                                        file_info["name_template"],
                                        full_data_dict)


def load_json(file_path):
    with open(file_path) as f:
        return json.load(f)


def load_component_data_description(file_path):
    component_data = load_json(file_path)

    for attribute in component_data["attributes"]:
        attribute["data_dict"] = get_attribute_data_dictionary(attribute)

    return component_data


def generate_all():
    for file_name in os.listdir(descriptions_dir):
        component = load_component_data_description(path.join(descriptions_dir, file_name))
        generate_files(files_to_generate, component)


delimiter_dictionary = load_json(path.join(configs_dir, "delimiter_dictionary.json"))

empty_delimiter_dictionary = {key: "" for key in delimiter_dictionary.keys()}

attribute_templates = load_json(path.join(configs_dir, "attribute_templates.json"))

files_to_generate = load_json(path.join(configs_dir, "files_to_generate.json"))

attribute_optional_fields = load_json(path.join(configs_dir, "attribute_optional_fields.json"))

generate_all()
