/*
 * QuickRank - A C++ suite of Learning to Rank algorithms
 * Webpage: http://quickrank.isti.cnr.it/
 * Contact: quickrank@isti.cnr.it
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * Contributors:
 *  - Andrea Battistini (andreabattistini@hotmail.com)
 *  - Chiara Pierucci (chiarapierucci14@gmail.com)
 *  - Claudio Lucchese (claudio.lucchese@isti.cnr.it)
 */
#pragma once

#include <memory>

#include "data/dataset.h"
#include "metric/ir/metric.h"
#include "learning/ltr_algorithm.h"

namespace quickrank {
namespace learning {
namespace linear {

/// This implements the Line Search algorithm.
class LineSearch: public LTR_Algorithm {

 public:

  LineSearch(unsigned int num_points, float window_size,
             float reduction_factor, unsigned int max_iterations,
             unsigned int max_failed_vali, bool adaptive,
             unsigned int last_only = 0);

  LineSearch(const pugi::xml_document &model);

  virtual ~LineSearch();

  /// Returns the name of the ranker.
  virtual std::string name() const {
    return NAME_;
  }

  unsigned int get_last_only() const {
    return train_only_last_;
  }

  void set_last_only(unsigned int last_only) {
    train_only_last_ = last_only;
  }

  void reset_weights() {
    best_weights_.clear();
  }

  static const std::string NAME_;

  /// Executes the learning process.
  ///
  /// \param training_dataset The training dataset.
  /// \param validation_dataset The validation training dataset.
  /// \param metric The metric to be optimized.
  /// \param partial_save Allows to save a partial model every given number of iterations.
  /// \param model_filename The file where the model, and the partial models, are saved.
  virtual void learn(std::shared_ptr<data::Dataset> training_dataset,
                     std::shared_ptr<data::Dataset> validation_dataset,
                     std::shared_ptr<metric::ir::Metric> metric,
                     size_t partial_save,
                     const std::string model_filename);

  /// Returns the score of a given document.
  virtual Score score_document(const Feature *d) const;

  /// Returns the learned weights
  virtual std::shared_ptr<std::vector<float>> get_weights() const {
    return std::shared_ptr<std::vector<float>>(
        new std::vector<float>(best_weights_));
  }

  virtual bool update_weights(std::shared_ptr<std::vector<float>> weights);

  /// Return the xml model representing the current object
  virtual pugi::xml_document *get_xml_model() const;

 private:
  unsigned int num_points_;
  float window_size_;
  float reduction_factor_;
  unsigned int max_iterations_;
  unsigned int max_failed_vali_;
  bool adaptive_;
  unsigned int train_only_last_;

  std::vector<float> best_weights_;

  /// The output stream operator.
  friend std::ostream &operator<<(std::ostream &os, const LineSearch &a) {
    return a.put(os);
  }

  /// Prints the description of Algorithm, including its parameters
  virtual std::ostream &put(std::ostream &os) const;

  virtual void preCompute(Feature *training_dataset,
                          unsigned int num_samples,
                          unsigned int num_features,
                          Score *pre_sum,
                          float *weights,
                          Score *training_score,
                          unsigned int feature_exclude);

  virtual void score(Feature *dataset, unsigned int num_samples,
                     unsigned int num_features, float *weights, Score *scores);
};

}  // namespace linear
}  // namespace learning
}  // namespace quickrank
