#include "crst.h"

const double EPS = 1e-7;

void copy_vector(const Vector_t from, Vector_t* to)
{
  to->x = from.x;
  to->y = from.y;
  to->z = from.z;
}

Dist_t get_dist(const Vector_t v1, const Vector_t v2)
{
  return sqrt( (v1.x - v2.x) * (v1.x - v2.x) +
               (v1.y - v2.y) * (v1.y - v2.y) +
               (v1.z - v2.z) * (v1.z - v2.z) );
}

void translate_new_vector(const Vector_t v0, const Vector_t trans_v, Vector_t* new_v)
{
  assert(new_v);

  new_v->x = v0.x + trans_v.x;
  new_v->y = v0.y + trans_v.y;
  new_v->z = v0.z + trans_v.z;
}

errno_t read_data(FILE* fdata, size_t* num_bs, Vector_t* bases, size_t* num_vt, Vector_t* trans, size_t* num_cs)
{
  assert(fdata);
  assert(num_bs);
  assert(bases);
  assert(num_vt);
  assert(trans);

  if (!fscanf(fdata, "%zu", num_bs))
  {
    errno = EINVAL;
    perror("num_bs");
    return errno;
  }

  bases = (Vector_t*)calloc(num_bs, sizeof(*bases));
  if (!bases)
  {
    errno = ENOMEM;
    perror("bases");
    return errno;
  }

  for (size_t num = 0; num < num_bs; num ++)
  {
    if (fscanf(fdata, "%lf,%lf,%lf", &bases[num].x, &bases[num].y, &bases[num].z) < 3)
    {
      errno = EINVAL;
      perror("bases");
      return errno;
    }
  }

  if (!fscanf(fdata, "%zu", num_vt))
  {
    errno = EINVAL;
    perror("num_vt");
    return errno;
  }

  trans = (Vector_t*)calloc(num_vt, sizeof(*trans));
  if (!trans)
  {
    errno = ENOMEM;
    perror("trans");
    return errno;
  }

  for (size_t num = 0; num < num_vt; num ++)
  {
    if (fscanf(fdata, "%lf,%lf,%lf", &trans[num].x, &trans[num].y, &trans[num].z) < 3)
    {
      errno = EINVAL;
      perror("trans");
      return errno;
    }
  }

  if (!fscanf(fdata, "%zu", num_cs))
  {
    errno = EINVAL;
    perror("num_cs");
    return errno;
  }

  return 0;
}

errno_t create_graph(Graph_t* graph, const size_t num_bs, Vector_t* bases, const size_t num_vt, Vector_t* trans, const size_t num_cs)
{
  assert(graph);
  assert(bases);
  assert(trans);

  graph = (Graph_t*)calloc(num_bs * num_vt * num_cs, sizeof(*graph));
  if (!graph)
  {
    errno = ENOMEM;
    perror("graph");
    return errno;
  }

  for (size_t num = 0; num < num_bs; num ++)
  {
    // --- Init graph --
    //
    // init g.v --- Вектор графа
    copy_vector(bases[num], &graph[num].v);
    // init g.bonds --- Связи узла с другими (Расстояние,Коорд.,Номер)
    graph[num].bonds = (Bond_t*)calloc(num_cs * num_vt * num_bs, sizeof(*graph[num].bonds));
    if (!graph[num].bonds)
    {
      errno = ENOMEM;
      perror("graph[num].bonds");
      return errno;
    }
    // init g.num_cood_s; g.size --- Нынешнее кол-во коорд. сфер
    graph[num].num_coord_s = 0;
    graph[num].size        = 0;


    // End init ------------------------------

    size_t size = graph[num].size;
    for (size_t othr = 0; othr < num_bs; othr ++)
    {
      if (othr == num) continue;
      size = graph[num].size;
      copy_vector(bases[othr], &graph[num].bonds[size].v_cs);
      graph[num].bonds[size].dist = get_dist(bases[othr],
                                    graph[num].bonds[size].v_cs);
      for (size_t com = 0; com < size; com ++)
      {
        if (fabs(graph[num].bonds[size].dist - graph[num].bonds.dist[com]) < EPS)
        {
          graph[num].bonds[size].num = graph[num].bonds.num[com];
          break;
        }
      }
      if (!graph[num].bonds[size].num)
      {
        graph[num].bonds[size].num = ++graph[num].num_coord_s;
      }
      graph[num].size++;
    }
  }
}

errno_t movings(Graph_t* graph, const size_t num_bs, const size_t num_vt, Vector_t* trans, const size_t num_cs)
{
  assert(graph);
  assert(trans);

  for (size_t node = 0; node < num_cs * num_vt * num_bs; node ++)
  {
    for (size_t move_v = 0; move_v < num_vt; move_v ++)
    {

    }
  }
}
