/* 
 * A Blocking Bounded Queue:
 * 1) Thread trying to remove an item from an empty queue will wait until an item is available ;
 * 2) Thread trying to put an item into a full queue will wait until there is room ;
