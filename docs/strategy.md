OPTIMISATIONS DONE:
  1) FOR THE BULK INSERTION:
      We have implemented bulk insertion not exactly just inserting record by record.
      Each block of records to be inserted are picked and performed the insertion.
      This reduces the no. of block accesses in the buffermanger.
