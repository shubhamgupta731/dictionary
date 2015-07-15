#include "Node.h"

tmb::BaseNodeFeatures::BaseNodeFeatures() {}

tmb::BaseNodeFeatures::BaseNodeFeatures(const tmb::BaseNodeFeatures& copy_from)
    : _name(copy_from._name)
    , _vec_functors_stream(copy_from._vec_functors_stream) {}

void tmb::BaseNodeFeatures::reset_dependencies() {
   typedef std::vector<std::vector<unsigned> > dependencies;
   for (dependencies::iterator it = _vec_dependencies.begin();
        it != _vec_dependencies.end();
        ++it) {
      std::fill(it->begin(), it->end(), 0);
   }
}

std::string& tmb::BaseNodeFeatures::get_name() { return _name; }

const std::vector<std::vector<std::string> >&
    tmb::BaseNodeFeatures::vector_of_strings() const {
   return _vec_subjects;
}

std::vector<std::vector<std::string> >
    tmb::BaseNodeFeatures::val_of_functor_wrappers() {
   std::vector<std::vector<std::string> > return_val;
   for (unsigned i = 0; i < _vec_functors_stream.size(); ++i) {
      return_val.push_back((*(_vec_functors_stream[i]))());
   }
   return return_val;
}

const std::vector<std::string>&
    tmb::BaseNodeFeatures::get_vector_dependencies_name() const {
   return _vec_dependency_name;
}

const std::vector<tmb::BaseNodeFeatures*>&
    tmb::BaseNodeFeatures::get_dependent_nodes() const {
   return _depends_on;
}

char space2underscore(char text) {
   if (text == ' ') {
      text = '_';
   }
   return text;
}

void tmb::draw_nodes(std::ofstream& fs,
                     tmb::BaseNodeFeatures* node,
                     unsigned levels,
                     unsigned count,
                     std::vector<std::string>& dictionary_of_nodes_added) {
   static char colors[10][10] = { "grey",   "grey10", "grey20", "grey30",
                                  "grey40", "grey50", "grey60", "grey70",
                                  "grey80", "grey90" };

   fs << "subgraph G" << node->get_name() << " {" << std::endl;
   std::vector<std::vector<std::string> > functor_args_vals =
       node->val_of_functor_wrappers();
   // convert space to underscore
   std::string original_node_name(node->get_name());
   std::transform(original_node_name.begin(),
                  original_node_name.end(),
                  original_node_name.begin(),
                  space2underscore);
   if (std::find(dictionary_of_nodes_added.begin(),
                 dictionary_of_nodes_added.end(),
                 original_node_name) == dictionary_of_nodes_added.end()) {
      fs << original_node_name << "[label=\"" << node->get_name() << " \\n";
      fs << "value: " << node->get_val_as_string() << "\"";
      fs << ",shape=\"rectangle\",style=filled,fillcolor=\"turquoise\"];"
         << "\n";
      dictionary_of_nodes_added.push_back(original_node_name);
   }
   fs << "}";
   unsigned original_count = count;
   for (unsigned i = 0; i < node->vector_of_strings().size(); ++i) {
      std::vector<std::string>& tokens(functor_args_vals[i]);
      fs << "subgraph cluster" << count << " {" << std::endl;
      fs << "style=filled;" << std::endl << "color=" << colors[count] << ";"
         << std::endl;
      for (unsigned j = 0; j < tokens.size(); ++j) {
         ++count;
         std::string node_name = node->vector_of_strings()[i][j];
         std::transform(node_name.begin(),
                        node_name.end(),
                        node_name.begin(),
                        space2underscore);
         if (std::find(dictionary_of_nodes_added.begin(),
                       dictionary_of_nodes_added.end(),
                       node_name) == dictionary_of_nodes_added.end()) {
            fs << node_name << "[label=\"" << node->vector_of_strings()[i][j]
               << " \\n";
            fs << "value: " << tokens[j] << "\"";
            fs << ",shape=\"rectangle\",style=filled,fillcolor=\"turquoise\"];"
               << "\n";
            dictionary_of_nodes_added.push_back(node_name);
         }
         fs << original_node_name << "->" << node_name << "[label=\""
            << node->get_vector_dependencies_name()[i] << "\"];" << std::endl;
      }
      fs << "}" << std::endl;
   }
   if (levels > 0) {
      const std::vector<tmb::BaseNodeFeatures*>& dependent_nodes =
          node->get_dependent_nodes();
      for (std::vector<tmb::BaseNodeFeatures*>::const_iterator it =
               dependent_nodes.begin();
           it != dependent_nodes.end();
           ++it) {
         draw_nodes(fs, *it, levels - 1, count, dictionary_of_nodes_added);
      }
   }
}
