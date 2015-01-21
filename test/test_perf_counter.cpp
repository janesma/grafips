// Copyright (C) Intel Corp.  2014.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#include <gtest/gtest.h>

#include <iostream>

#include "test/test_gpu_context.h"
#include "sources/gfgpu_perf_functions.h"

using Grafips::MockContext;
using Grafips::PerfFunctions;

GLint max_name_len = 0, max_counter_name_len = 0, max_desc_len = 0;

void test_gpu_timestamp(MockContext *context, GLuint query_id, GLuint counter_num,
                        GLuint counter_offset, GLuint counter_data_size,
                        GLuint counter_type_enum,
                        GLuint counter_data_type_enum) {
  std::vector<unsigned int> extant_query_handles;
  std::vector<unsigned int> free_query_handles;
  std::vector<unsigned char> data_buf;
  std::vector<GLchar> query_name(max_name_len);
  GLuint data_size, num_counters, num_instances, capabilities_mask;

  EXPECT_EQ(GL_PERFQUERY_COUNTER_RAW_INTEL, counter_type_enum);
  EXPECT_EQ(GL_PERFQUERY_COUNTER_DATA_UINT64_INTEL, counter_data_type_enum);

  //uint64_t counter_val = 0;
  for (int i = 0; i < 10; ++i) {
    
    //test that the number of queries is correct
    PerfFunctions::GetQueryInfo(query_id, query_name.size(),
                                query_name.data(), &data_size,
                                &num_counters, &num_instances,
                                &capabilities_mask);
    EXPECT_EQ(num_instances, extant_query_handles.size());
    data_buf.resize(data_size);
    for (auto extant_query = extant_query_handles.rbegin();
         extant_query != extant_query_handles.rend(); ++extant_query) {
      GLuint bytes_written = 0;
      PerfFunctions::GetQueryData(*extant_query, GL_PERFQUERY_DONOT_FLUSH_INTEL,
                                  data_size, data_buf.data(),
                                  &bytes_written);
      if (bytes_written != 0)
        continue;

      //uint64_t new_val = *((uint64_t *)(data_buf.data() + counter_offset));
      //EXPECT_GT(new_val, counter_val);
      //counter_val = new_val;
      free_query_handles.push_back(*extant_query);
      *extant_query = extant_query_handles.back();
      extant_query_handles.pop_back();
    }

    if (free_query_handles.empty()) {
      unsigned int query_handle;
      PerfFunctions::CreateQuery(query_id, &query_handle);
      assert(query_handle != GL_INVALID_VALUE);
      free_query_handles.push_back(query_handle);
    }

    GLuint current_query_handle = free_query_handles.back();
    free_query_handles.pop_back();

    PerfFunctions::BeginQuery(current_query_handle);
    context->Draw();
    PerfFunctions::EndQuery(current_query_handle);
    extant_query_handles.push_back(current_query_handle);
  }
}

TEST(perf_counter, list) {
  MockContext m;
  GLuint id = 0;
  PerfFunctions::GetFirstQueryId(&id);
  std::vector<int> ids;
  ASSERT_NE(id, 0);
  ids.push_back(id);

  while (true) {
    PerfFunctions::GetNextQueryId(id, &id);
    if (!id)
      break;
    ids.push_back(id);
  }

  GLuint data_size, num_counters, num_instances, capabilities_mask;
  glGetIntegerv(GL_PERFQUERY_QUERY_NAME_LENGTH_MAX_INTEL, &max_name_len);
  glGetIntegerv(GL_PERFQUERY_COUNTER_NAME_LENGTH_MAX_INTEL, &max_counter_name_len);
  glGetIntegerv(GL_PERFQUERY_COUNTER_DESC_LENGTH_MAX_INTEL, &max_desc_len);

  std::vector<GLchar> query_name(max_name_len);
  EXPECT_EQ(ids.size(), 2);
  for (auto i = ids.begin(); i != ids.end(); ++i) {
    PerfFunctions::GetQueryInfo(*i, query_name.size(),
                                query_name.data(), &data_size,
                                &num_counters, &num_instances,
                                &capabilities_mask);
    std::cout << "query_name: " << query_name.data()
              << ", instances: " << num_instances
              << ", data_size: " << data_size << std::endl;
    
    for (unsigned int j = 1; j <= num_counters; ++j) {
      std::vector<GLchar> counter_name(max_counter_name_len);
      std::vector<GLchar> counter_description(max_desc_len);
      GLuint counter_offset, counter_data_size,
          counter_type_enum,
          counter_data_type_enum;
      GLuint64 raw_counter_max_value;
      PerfFunctions::GetCounterInfo(*i, j,
                                    max_counter_name_len, counter_name.data(),
                                    max_desc_len, counter_description.data(),
                                    &counter_offset, &counter_data_size,
                                    &counter_type_enum,
                                    &counter_data_type_enum,
                                    &raw_counter_max_value);
      std::cout << "counter_name: " << counter_name.data() << std::endl;
      std::cout << "counter_desc: " << counter_description.data() << std::endl;
      EXPECT_EQ(strcmp(counter_name.data(), counter_description.data()), 0);
      if (strcmp(counter_name.data(), "GPU Timestamp") == 0) {
        //test_gpu_timestamp(&m, *i, j, counter_offset, counter_data_size, counter_type_enum, counter_data_type_enum);
        continue;
      }

      if (strcmp(counter_name.data(), "EU Active") == 0) {
        //test_eu_active(&m, *i, j, counter_offset, counter_data_size, counter_type_enum, counter_data_type_enum);
        continue;
      }
}
  }
}
