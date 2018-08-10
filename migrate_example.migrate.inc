<?php

function migrate_example_migrate_api() {

	$api = array(
		'api' => 2,
		'groups' => array(
			'cgtt_product' => array(
				'title' => t('CGTT products'),
			),
		),
		'migrations' => array(
			'Projects' => array(
				'class_name' => 'ProjectMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Subject' => array(
				'class_name' => 'SubjectTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Requeriments' => array(
				'class_name' => 'RequerimentTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Companies' => array(
				'class_name' => 'CompanyTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Consejeria' => array(
				'class_name' => 'ConsejeriaTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'PublicVersion' => array(
				'class_name' => 'PublicVersionMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'PrivateVersion' => array(
				'class_name' => 'PrivateVersionMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Tag' => array(
				'class_name' => 'TagTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'NewSubject' => array(
				'class_name' => 'NewSubjectMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'News' => array(
				'class_name' => 'NewsMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Documentation' => array(
				'class_name' => 'DocumentationMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Tipology' => array(
				'class_name' => 'TipologyMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
		),
	);

	return $api;


}


function migrate_customtable_migrate_api() {

	$api = array(
		'api' => 2,
		'groups' => array(
			'custom_table' => array(
				'title' => t('Custom Table'),
			),
		),
		'migrations' => array(
			'Projects' => array(
				'class_name' => 'CustomTableMigration',
				'group_name' => 'custom_table',
				'event_log' => ''
			),

		),
	);

	return $api;


}