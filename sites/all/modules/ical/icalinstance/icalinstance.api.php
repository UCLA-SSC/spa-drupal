<?php

/**
 * Hook for acting on the creation of new instance nodes.
 *
 * @param array $nids
 *   The node IDs of the nodes that have just been created.  Note that these
 *   may not all be from the same vevent or rrule, but will all be from the
 *   same event node.
 * @param $node
 *   The node object with icalfield on it that is the origin of these instances.
 */
function hook_icalinstance_create($nids, $node) {

}

/**
 * Hook for acting on instances nodes that are about to be pruned.
 *
 * Pruned nodes will be deleted, but this hook fires before the nodes are deleted
 * so that any data that needs to be pulled from them is still available.
 *
 * @param $nids
 *   The node IDs of the nodes that are about to be deleted.  Note that these
 *   may not all be from the same vevent or rrule, but will all be from the
 *   same event node.
 * @param $node
 *   The node object with icalfield on it that is the origin of these instances.
 */
function hook_icalinstance_prune($nids, $node) {

}

/**
 * Hook for acting on instances nodes that have just been re-saved.
 *
 * @param $nids
 *   The node IDs of the nodes that have just been resaved.  Note that these
 *   may not all be from the same vevent or rrule, but will all be from the
 *   same event node.
 * @param $node
 *   The node object with icalfield on it that is the origin of these instances.
 */
function hook_icalinstance_update($nids, $node) {

}

/**
 * Hook for acting on instances nodes that have just been saved.
 *
 * "Saved" nodes are the union of both the create and update event hooks.
 *
 * @param $nids
 *   The node IDs of the nodes that have just been saved.  Note that these
 *   may not all be from the same vevent or rrule, but will all be from the
 *   same event node.
 * @param $node
 *   The node object with icalfield on it that is the origin of these instances.
 */
function hook_icalinstance_save($nids, $node) {

}

